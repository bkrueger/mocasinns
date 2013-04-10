LIBFILES=../lib/libising.a
GOPT=-ggdb
CC=g++
CFLAGS=$(GOPT) -std=c++0x -Wall -Wextra -pedantic -I ../include

TEST_OBJECTS_MAIN = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TEST_OBJECTS_SPINS = $(patsubst %.cpp,%.o,$(wildcard test_spins/*.cpp))
TEST_OBJECTS = $(TEST_OBJECTS_MAIN) $(TEST_OBJECTS_SPINS)

test: test.o $(TEST_OBJECTS)
	g++ $(GOPT) -o test $(TEST_OBJECTS) $(LIBFILES) -lboost_serialization -lcppunit -ldl

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_OBJECTS) test