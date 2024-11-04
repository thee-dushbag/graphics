
CC=g++ -std=c++17
ROOTDIR=../..
INCDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib 
LIBS=-lm -lGL -lX11 -lpng16 -Wl,-rpath,$(LIBDIR) -L$(LIBDIR) -lolcPGE


all: main vecs

main: main.cpp simulator.hpp
	$(MAKE) -C $(LIBDIR) libolcPGE.so
	$(CC) -o $@ main.cpp -I$(INCDIR) $(LIBS)

vecs: vecs.cpp
	$(MAKE) -C $(LIBDIR) libolcPGE.so
	$(CC) -o $@ vecs.cpp -I$(INCDIR) $(LIBS)

.PHONY+=clean
clean:
	rm -f main vecs
	$(MAKE) -C $(LIBDIR) clean

