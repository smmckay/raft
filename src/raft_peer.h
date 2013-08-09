#include <set>
#include <random>
#include <thread>
#include <chrono>
#include <asio.hpp>
#include <boost/log/trivial.hpp>

#include "peer_connection.h"

using asio::ip::tcp;

class raft_peer
{
public:
	raft_peer(asio::io_service &io,
			int local_port,
			const std::set<int> &peer_ports)
		: _io(io),
		_acceptor(io, tcp::endpoint(tcp::v4(), local_port)),
		_peer_ports(peer_ports)
	{
		std::seed_seq seq = {
			static_cast<int>(std::hash<std::thread::id>()(std::this_thread::get_id())),
			static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count())
		};
		_rand_gen.seed(seq);

		BOOST_LOG_TRIVIAL(info) << "Listening on port " << local_port;
		_acceptor.listen();
		start_accept();
		start_connect();
	};

private:
	void start_accept();
	void handle_accept(peer_connection::pointer peer,
			const asio::error_code &error);
	void start_connect();
	void start_connect(int port);
	void handle_connect(peer_connection::pointer peer,
			const asio::error_code &error);
	void add_active_peer(peer_connection::pointer peer);

	asio::io_service &_io;
	tcp::acceptor _acceptor;
	std::set<int> _peer_ports;
	std::map<int, peer_connection::pointer> _active_peers;
	std::mt19937 _rand_gen;
};
