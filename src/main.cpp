#include <gflags/gflags.h>
#include <glog/logging.h>
#include <vector>

#include "ostream_ops.cpp"

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
	FLAGS_logtostderr = 1;
	google::InitGoogleLogging(argv[0]);

	LOG(INFO) << "Inited successfully. Our port is " << FLAGS_port;

	std::vector<int> peer_ports;
	for (int i = 10000; i < 10004; i++) {
		if (i != FLAGS_port) {
			peer_ports.push_back(i);
		}
	}
	LOG(INFO) << "Peer ports are " << peer_ports;

	return 0;
}

