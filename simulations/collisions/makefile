
CC=g++ -std=c++17
ROOTDIR=../..
INCDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib 
LIBS=-lm -lGL -lX11 -lpng16 -Wl,-rpath,$(LIBDIR) -L$(LIBDIR) -lolcPGE

ifneq ($(BALLS),)
CC+=-DBALLS=$(BALLS)
endif

ifneq ($(VELOCITY),)
CC+=-DVELOCITY=$(VELOCITY)
endif

ifneq ($(SIMH),)
CC+=-DSIMH=$(SIMH)
endif

ifneq ($(SIMW),)
CC+=-DSIMW=$(SIMW)
endif

ifeq ($(MESH),true)
CC+=-DMESH=1 
endif

ifneq ($(RADIUS),)
CC+=-DRADIUS=$(RADIUS)
endif

all: main vecs

main: main.cpp simulator.hpp
	$(MAKE) -C $(LIBDIR) libolcPGE.so
	$(CC) -o $@ $< -I$(INCDIR) $(LIBS)

vecs: vecs.cpp
	$(MAKE) -C $(LIBDIR) libolcPGE.so
	$(CC) -o $@ $< -I$(INCDIR) $(LIBS)

.PHONY+=clean
clean:
	rm -f main vecs
	$(MAKE) -C $(LIBDIR) clean

