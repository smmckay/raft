AM_CXXFLAGS = -std=c++11 -fexceptions -Wall -Wextra -Werror
if CLANG
AM_CXXFLAGS += -stdlib=libc++
endif
AM_CPPFLAGS = -I$(top_srcdir)/src -DBOOST_ALL_DYN_LINK
AM_LDFLAGS = -lboost_system-mt -lboost_log-mt

bin_PROGRAMS = src/raft_peer
src_raft_peer_SOURCES = src/peer_connection.cpp src/raft_peer.cpp src/main.cpp \
			src/gflags.cc src/gflags_reporting.cc \
			src/gflags_completions.cc

