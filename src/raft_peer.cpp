#include "raft_peer.h"
#include "peer_connection.h"

void raft_peer::start_accept()
{
	auto new_connection = peer_connection::create(_io);
	_acceptor.async_accept(new_connection->socket(),
			[&new_connection, this] (const boost::system::error_code& error) {
				if (!error) {
					new_connection->start();
				}
				start_accept();
			});
}

