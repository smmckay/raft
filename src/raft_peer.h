#include <set>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class raft_peer
{
public:
	raft_peer(boost::asio::io_service &io,
			int local_port,
			const std::set<int> & /*peer_ports*/)
		: _io(io),
		_acceptor(io, tcp::endpoint(tcp::v4(), local_port))
	{
		_acceptor.listen();
		start_accept();
	};

private:
	void start_accept();

	boost::asio::io_service &_io;
	tcp::acceptor _acceptor;
};
