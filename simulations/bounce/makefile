CC=g++ -std=c++17

ROOTDIR=../..
INCDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib 

LIBS=-lm -lX11 -lGL -lpng16 -Wl,-rpath,$(LIBDIR) -L$(LIBDIR) -lolcPGE

main: main.cpp bounce.cpp
	$(MAKE) --no-print-directory -C $(LIBDIR) libolcPGE.so
	$(CC) -o $@ main.cpp -I$(INCDIR) $(LIBS)

.PHONY+=olcPGE_Library
olcPGE_Library:
	$(MAKE) --no-print-directory -C $(LIBDIR) libolcPGE.so

.PHONY+=clean
clean:
	rm -f main
	$(MAKE) --no-print-directory -C $(LIBDIR) clean

