CC=g++ -O3 -std=c++17
INCDIR=../include
VPATH+=$(INCDIR)


libolcPGE.so: olcPGE.cpp olcPixelGameEngine.h
	$(CC) -shared -fPIC -o $@ olcPGE.cpp -I$(INCDIR)

.PHONY+=clean
clean:
	rm -f libolcPGE.so

