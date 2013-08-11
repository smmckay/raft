#include <functional>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "raft_peer.h"
#include "peer_connection.h"

using namespace std::placeholders;

void raft_peer::start_accept()
{
	auto new_connection = peer_connection::create(_io);
	_acceptor.async_accept(new_connection->socket(),
			std::bind(&raft_peer::handle_accept, this, new_connection, _1));
}

void raft_peer::handle_accept(peer_connection::pointer peer,
		const asio::error_code& error)
{
	if (!error) {
		LOG(INFO) << "Received connection from "
			<< peer->socket().remote_endpoint().port();
		add_active_peer(peer);
	} else {
		LOG(WARNING) << "async_accept failed: " << error.message();
	}
	start_accept();
}

void raft_peer::start_connect()
{
	for (const auto &port: _peer_ports) {
		start_connect(port);
	}
}

void raft_peer::start_connect(int port)
{
	LOG(INFO) << "async_connect to " << port;
	auto new_connection = peer_connection::create(_io);
	asio::ip::tcp::endpoint peer_endpoint(
			asio::ip::address_v4::any(), port);
	new_connection->socket().async_connect(peer_endpoint,
			std::bind(&raft_peer::handle_connect, this, new_connection, port, _1));
}

void raft_peer::handle_connect(peer_connection::pointer new_connection, int port,
		const asio::error_code& error)
{
	if (!error) {
		int peer_port = new_connection->socket().remote_endpoint().port();
		LOG(INFO) << "Connected to " << peer_port;
		add_active_peer(new_connection);
	} else {
		std::uniform_int_distribution<> dist(500, 750);
		int delay = dist(_rand_gen);
		LOG(WARNING) << "Connection to port " << port << " failed: "
			<< error.message() << ". Retrying in " << delay << "ms";
		new_connection->close();

		if (!_retry_timers[port]) {
			_retry_timers[port].reset(new asio::deadline_timer(_io,
						boost::posix_time::milliseconds(delay)));
		} else {
			_retry_timers[port]->expires_from_now(
					boost::posix_time::milliseconds(delay));
		}
		_retry_timers[port]->async_wait([this, port](const asio::error_code& error) {
			start_connect(port);
			if (error) {
				LOG(ERROR) << "Timer failed: " << error.message();
			}
		});
	}
}

void raft_peer::add_active_peer(peer_connection::pointer peer)
{
	int port = peer->socket().remote_endpoint().port();
	auto result = _active_peers.insert(std::pair<int, peer_connection::pointer>(port, peer));
	if (result.second) {
		peer->start();
	} else {
		LOG(INFO) << "Existing connection to " << port
			<< ", closing new connection";
		peer->close(true);
	}
}

