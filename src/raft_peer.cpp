#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <string.h>
#include <glog/logging.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

#include "raft_peer.h"

raft_peer::raft_peer(int local_port, const std::set<int> &peer_ports) noexcept
	: _peer_ports(peer_ports)
{
	_socket = nn_socket(AF_SP, NN_BUS);
	CHECK_GE(_socket, 0) << "nn_socket: " << nn_strerror(errno);

	_kqueue = kqueue();
	CHECK_GE(_kqueue, 0) << "kqueue: " << strerror(errno);

	char addr_buf[NN_SOCKADDR_MAX];
	std::snprintf(addr_buf, NN_SOCKADDR_MAX, "tcp://*:%d", local_port);
	int err = nn_bind(_socket, addr_buf);
	CHECK_GE(err, 0) << "nn_bind: " << nn_strerror(errno);
	LOG(INFO) << "Listening on port " << local_port;

	for (const int &port : peer_ports) {
		if (port <= local_port) {
			continue;
		}
		std::snprintf(addr_buf, NN_SOCKADDR_MAX,
				"tcp://localhost:%d", port);
		err = nn_connect(_socket, addr_buf);
		CHECK_GE(err, 0) << "nn_connect: " << nn_strerror(errno);
		LOG(INFO) << "Connecting to port " << port;
	}

	struct kevent events[2];
	int rcvfd;
	size_t int_len = sizeof(rcvfd);
	err = nn_getsockopt(_socket, NN_SOL_SOCKET, NN_RCVFD, &rcvfd, &int_len);
	CHECK_GE(err, 0) << "nn_getsockopt: " << nn_strerror(errno);
	EV_SET(&events[0], rcvfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	EV_SET(&events[1], PING_TIMER, EVFILT_TIMER, EV_ADD, 0, 1000, NULL);
	err = kevent(_kqueue, events, 2, NULL, 0, NULL);
	CHECK_GE(err, 0) << "kevent: " << strerror(errno);
}

raft_peer::~raft_peer()
{
	int err;
	do {
		err = nn_close(_socket);
	} while (err < 0 && errno == EINTR);
	if (err < 0) {
		LOG(WARNING) << "nn_close failed: " << nn_strerror(errno);
	}
}


void raft_peer::loop()
{
	struct kevent events[2];
	while (true) {
		int nevents = kevent(_kqueue, NULL, 0, events, 3, NULL);
		CHECK_GE(nevents, 0) << "kqueue failed: " << strerror(errno);
		for (int i = 0; i < nevents; i++) {
			switch (events[i].filter) {
			case EVFILT_TIMER:
				LOG(INFO) << "tick " << events[i].data;
				break;
			case EVFILT_READ:
				LOG(INFO) << "msg in";
				break;
			}
		}
	}
}

