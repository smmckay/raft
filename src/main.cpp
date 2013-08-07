#include <gflags/gflags.h>
#include <set>
#include <asio.hpp>

#include "ostream_ops.h"
#include "raft_peer.h"

static bool ValidatePort(const char* flagname, google::int32 value) {
   if (value > 9999 && value < 10004)   // value is ok
     return true;
   printf("Invalid value for --%s: %d\n", flagname, (int)value);
   return false;
}

DEFINE_int32(port, 0, "What port to listen on");
static const bool port_dummy = google::RegisterFlagValidator(&FLAGS_port, &ValidatePort);


int main(int argc, char **argv)
{
	google::ParseCommandLineFlags(&argc, &argv, true);

	std::set<int> peer_ports = {10000, 10001, 10002, 10003};
	peer_ports.erase(FLAGS_port);

	asio::io_service io;
	try {
		raft_peer peer(io, FLAGS_port, peer_ports);
		io.run();
	} catch (std::exception &e) {
		BOOST_LOG_TRIVIAL(error) << "Caught exception: " << e.what();
	}
	return 0;
}

