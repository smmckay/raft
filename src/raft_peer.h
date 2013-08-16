#include <set>

class raft_peer
{
public:
	raft_peer(int local_port, const std::set<int> &peer_ports) noexcept;
	~raft_peer();
	void loop();
private:
	std::set<int> _peer_ports;
	int _socket;
	int _kqueue;
	const int PING_TIMER = 1;
};

