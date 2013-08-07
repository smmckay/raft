#pragma once

#include <asio.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>

class peer_connection
	: public boost::enable_shared_from_this<peer_connection>
{
public:
	typedef boost::shared_ptr<peer_connection> pointer;

	static pointer create(asio::io_service &io);
	asio::ip::tcp::socket& socket();
	void start();
	void close(bool shutdown = false);
private:
	peer_connection(asio::io_service &io)
		: _socket(io) { }
	void handle_write(const asio::error_code &error, size_t bytes_written);

	void read_length();
	void read_message(const asio::error_code &error, size_t bytes_written);
	void handle_message(const asio::error_code &error, size_t bytes_written);

	asio::ip::tcp::socket _socket;
	boost::uint32_t _receive_len;
	boost::uint8_t _receive_buf[4096];
};

