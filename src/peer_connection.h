#pragma once

#include <boost/asio.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>

class peer_connection
	: public boost::enable_shared_from_this<peer_connection>
{
public:
	typedef boost::shared_ptr<peer_connection> pointer;

	static pointer create(boost::asio::io_service &io);
	boost::asio::ip::tcp::socket& socket();
	void start();
private:
	peer_connection(boost::asio::io_service &io)
		: _socket(io) { }
	void handle_write(const boost::system::error_code &error, size_t bytes_written);

	void read_length();
	void read_message(const boost::system::error_code &error, size_t bytes_written);
	void handle_message(const boost::system::error_code &error, size_t bytes_written);

	boost::asio::ip::tcp::socket _socket;
	boost::uint32_t _receive_len;
	boost::uint8_t _receive_buf[4096];
};

