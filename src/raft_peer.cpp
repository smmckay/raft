#include <functional>

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
		BOOST_LOG_TRIVIAL(debug) << "Received connection from "
			<< peer->socket().remote_endpoint().port();
		add_active_peer(peer);
	} else {
		BOOST_LOG_TRIVIAL(warning) << "async_accept failed: " << error.message();
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
	BOOST_LOG_TRIVIAL(debug) << "async_connect to " << port;
	auto new_connection = peer_connection::create(_io);
	asio::ip::tcp::endpoint peer_endpoint(
			asio::ip::address_v4::any(), port);
	new_connection->socket().async_connect(peer_endpoint,
			std::bind(&raft_peer::handle_connect, this, new_connection, _1));
}

void raft_peer::handle_connect(peer_connection::pointer new_connection,
		const asio::error_code& error)
{
	if (!error) {
		int peer_port = new_connection->socket().remote_endpoint().port();
		BOOST_LOG_TRIVIAL(debug) << "Connected to " << peer_port;
		add_active_peer(new_connection);
	} else {
		BOOST_LOG_TRIVIAL(warning) << "Connection failed: " << error.message();
		new_connection->close();
	}
}

void raft_peer::add_active_peer(peer_connection::pointer peer)
{
	int port = peer->socket().remote_endpoint().port();
	auto result = _active_peers.insert(std::pair<int, peer_connection::pointer>(port, peer));
	if (result.second) {
		peer->start();
	} else {
		BOOST_LOG_TRIVIAL(debug) << "Existing connection to " << port
			<< ", closing new connection";
		peer->close(true);
	}
}

