CC = gcc
CXX = g++
CFLAGS = -g -pg -O2 -Wall -Wextra -pedantic -std=gnu++0x -fPIC
INCLUDE = -I../include

LIBRND_OBJECTS = random_boost_mt19937.o

all: library

library: $(LIBRND_OBJECTS)
	$(CXX) -shared $(LIBRND_OBJECTS) -o ../lib/librandom.so
	ar -cvq ../lib/librandom.a $(LIBRND_OBJECTS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

clean:
	rm -f *.o
	rm ../lib/librandom.a
	rm ../lib/librandom.so
