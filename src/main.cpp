#include <set>
#include <cstdlib>
#include <gflags/gflags.h>
#include <glog/logging.h>

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
	google::InitGoogleLogging(argv[0]);
	google::InstallFailureSignalHandler();
	google::InstallFailureFunction([]() { std::exit(EXIT_FAILURE); });
	FLAGS_logtostderr = 1;

	std::set<int> peer_ports = {10000, 10001, 10002, 10003};
	peer_ports.erase(FLAGS_port);

	try {
		raft_peer peer(FLAGS_port, peer_ports);
		peer.loop();
	} catch (std::exception &e) {
		LOG(ERROR) << "Caught exception: " << e.what();
	}
	return 0;
}

