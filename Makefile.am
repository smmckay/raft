SUBDIRS = glog-0.3.3 gflags-2.0

AM_CXXFLAGS = -std=c++11 -fexceptions -Wall -Wextra -Werror
AM_CPPFLAGS = -I$(top_srcdir)/gflags-2.0/src \
	      -I$(top_srcdir)/glog-0.3.3/src \
	      -I$(top_srcdir)/libuv/include \
	      -DBOOST_ALL_DYN_LINK
AM_LDFLAGS = -lboost_system-mt

bin_PROGRAMS = src/raft_peer
src_raft_peer_SOURCES = src/peer_connection.cpp src/raft_peer.cpp src/main.cpp
src_raft_peer_LDADD = gflags-2.0/.libs/libgflags.a glog-0.3.3/.libs/libglog.a
