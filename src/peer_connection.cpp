#include <functional>
#include <glog/logging.h>

#include "peer_connection.h"

using namespace std::placeholders;

peer_connection::pointer peer_connection::create(asio::io_service &io)
{
	return pointer(new peer_connection(io));
}

asio::ip::tcp::socket& peer_connection::socket()
{
	return _socket;
}

void peer_connection::start()
{
	read_length();
}

void peer_connection::close(bool shutdown)
{
	asio::error_code ec;
	if (shutdown) {
		_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		if (ec) {
			LOG(WARNING) << "socket shutdown failed: " << ec.message();
		}
	}
	_socket.close(ec);
	if (ec) {
		LOG(WARNING) << "socket close failed: " << ec.message();
	}
}

void peer_connection::handle_write(const asio::error_code&, size_t)
{

}

void peer_connection::read_length()
{
	asio::async_read(_socket,
		asio::buffer(&_receive_len, sizeof(_receive_len)),
		std::bind(&peer_connection::read_message, this, _1, _2));
}

void peer_connection::read_message(const asio::error_code& error, std::size_t /*bytes_transferred*/)
{
	if (!error) {
		asio::async_read(_socket,
			asio::buffer(_receive_buf, sizeof(_receive_buf)),
			std::bind(&peer_connection::handle_message, this, _1, _2));
	} else {
		// log and cleanup
	}
}

void peer_connection::handle_message(const asio::error_code& /* error */, std::size_t /*bytes_transferred*/)
{
	// do something
	read_length();
}

