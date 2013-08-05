#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class peer_connection
	: public boost::enable_shared_from_this<peer_connection>
{
public:
	typedef boost::shared_ptr<peer_connection> pointer;

	static pointer create(boost::asio::io_service &io);
	tcp::socket& socket();
	void start();
private:
	peer_connection(boost::asio::io_service &io)
		: _socket(io) { }
	void handle_write(const boost::system::error_code &error,
			size_t bytes_written);
	tcp::socket _socket;
};

