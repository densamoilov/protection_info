libname := cryptolib.so
lib_obj := cryptolib.o
lib_src := cryptolib.cpp

progname := test
prog_src := test.cpp
prog_obj := test.o

CC := g++
CFLAGS := -O3 -Wall -g -std=c++14 -fopenmp
LDFLAGS := -fopenmp -L. cryptolib.so

all: $(progname)
	

$(progname): $(prog_obj) $(libname)
	$(CC) -o $(progname) $(prog_obj) $(LDFLAGS)

$(libname): $(lib_obj)
	$(CC) -shared -o $(libname) $(lib_obj)

$(prog_obj): $(prog_src)
	$(CC) $(CFLAGS) -c $(prog_src) -o $@

$(lib_obj): $(lib_src)
	$(CC) $(CFLAGS) -fPIC -c $(lib_src) -o $@

clean:
	rm -f *.o *.so $(progname) 