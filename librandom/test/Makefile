CC = gcc
CXX = g++
CFLAGS = -g -pg -O2 -Wall -Wextra -pedantic -std=gnu++0x
LDFLAGS = -L../lib
INCLUDE = -I../include

TEST_LIBS = -lrandom -lcppunit -ldl -lboost_random
TEST_OBJECTS = test_boost_mt19937.o test.o

all: test

test: $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(TEST_OBJECTS) -o test $(TEST_LIBS) 

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

clean:
	rm -f *.o test
