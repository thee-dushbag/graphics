CC=g++ -O3 -std=c++17
INCDIR=../include
VPATH+=$(INCDIR)


libolcPGE.so: olcPGE.cpp olcPixelGameEngine.hpp
	$(CC) -shared -fPIC -o $@ olcPGE.cpp -I$(INCDIR)

.PHONY+=clean
clean:
	rm -f libolcPGE.so

