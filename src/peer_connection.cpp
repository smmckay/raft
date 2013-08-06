#include <functional>

#include "peer_connection.h"

using namespace std::placeholders;

peer_connection::pointer peer_connection::create(boost::asio::io_service &io)
{
	return pointer(new peer_connection(io));
}

boost::asio::ip::tcp::socket& peer_connection::socket()
{
	return _socket;
}

void peer_connection::start()
{
	read_length();

}

void peer_connection::handle_write(const boost::system::error_code&, size_t)
{

}

void peer_connection::read_length()
{
	boost::asio::async_read(_socket,
		boost::asio::buffer(&_receive_len, sizeof(_receive_len)),
		std::bind(&peer_connection::read_message, this, _1, _2));
}

void peer_connection::read_message(const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
{
	if (!error) {
		boost::asio::async_read(_socket,
			boost::asio::buffer(_receive_buf, sizeof(_receive_buf)),
			std::bind(&peer_connection::handle_message, this, _1, _2));
	} else {
		// log and cleanup
	}
}

void peer_connection::handle_message(const boost::system::error_code& /* error */, std::size_t /*bytes_transferred*/)
{
	// do something
	read_length();
}

