#include "peer_connection.h"

peer_connection::pointer peer_connection::create(boost::asio::io_service &io)
{
	return pointer(new peer_connection(io));
}

tcp::socket& peer_connection::socket()
{
	return _socket;
}

void peer_connection::start()
{

}

void peer_connection::handle_write(const boost::system::error_code&, size_t)
{

}

