CC       = g++
OPT	 = -ggdb
CFLAGS   = $(OPT) -fPIC -Wall -Wextra -pedantic
OBJ_LIB  = spin_ising.o spin_potts.o spin_real.o spinlattice.o step.o
OBJ_COMP = $(OBJ_LIB) template_compile_test.o

all: library

library: $(OBJ_COMP)
	$(CC) -shared $(OBJ_LIB) -o ../lib/libising.so
	ar -cvq ../lib/libising.a  $(OBJ_LIB)

python_library : $(OBJS)
	$(CC) $(CFLAGS) -I/usr/include/python2.6 -lboost_python -c libising_wrap.cpp -o libising_wrap.o
	$(CC) -shared $(CFLAGS) $(OBJ_LIB) -lboost_python libising_wrap.o -o ../lib/libising_python.so

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ_COMP)
	rm ../lib/libising.so
	rm ../lib/libising_python.so
	rm ../lib/libising.a