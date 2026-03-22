# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

all: shell test delay

shell: main.o apishell.o
	$(CXX) $(CXXFLAGS) -o shell main.o apishell.o

test: test.o apishell.o
	$(CXX) $(CXXFLAGS) -o test test.o apishell.o

delay: delay.cpp
	$(CXX) $(CXXFLAGS) -o delay delay.cpp

main.o: main.cpp apishell.h
	$(CXX) $(CXXFLAGS) -c main.cpp

test.o: test.cpp apishell.h
	$(CXX) $(CXXFLAGS) -c test.cpp

apishell.o: apishell.cpp apishell.h
	$(CXX) $(CXXFLAGS) -c apishell.cpp

clean:
	rm -f *.o shell test delay

.PHONY: all clean shell test delay