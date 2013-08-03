CXX = clang++
CXXFLAGS = -std=c++11 -fexceptions -Wall -Wextra -Werror -g
LDFLAGS = -lglog -lgflags

OBJS = main.o ostream_ops.o

.PHONY: all clean
all: main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) main
