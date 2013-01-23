LIBFILES=../lib/libising.a
TESTFILES=spinisingtest.o spinpottstest.o spinrealtest.o spinlatticetest.o steptest.o
GOPT=-ggdb
CC=g++
CFLAGS=$(GOPT) -Wall -Wextra -pedantic

test: test.o $(TESTFILES)
	g++ $(GOPT) -o test test.o $(TESTFILES) $(LIBFILES) -lboost_serialization -lcppunit -ldl

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o test