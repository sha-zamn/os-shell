CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

all: apishell test delay

apishell: main.cpp apishell.cpp circular_buffer.h apishell.h
	$(CXX) $(CXXFLAGS) -o apishell main.cpp apishell.cpp circular_buffer.h

test: test.cpp apishell.cpp circular_buffer.h apishell.h
	$(CXX) $(CXXFLAGS) -o test test.cpp apishell.cpp circular_buffer.h

delay: delay.cpp
	$(CXX) $(CXXFLAGS) -o delay delay.cpp

clean:
	rm -f *.o apishell test delay

.PHONY: all clean apishell test delay