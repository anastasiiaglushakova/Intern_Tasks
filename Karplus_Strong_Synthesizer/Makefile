CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra

all: karplus_strong

karplus_strong: main.o karplus_strong.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp karplus_strong.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

karplus_strong.o: karplus_strong.cpp karplus_strong.hpp
	$(CXX) $(CXXFLAGS) -c karplus_strong.cpp

clean:
	rm -f *.o
