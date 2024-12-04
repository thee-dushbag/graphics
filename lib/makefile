CXX=g++
INCDIR=../include
VPATH+=$(INCDIR)
CXXFLAGS=-02

libolcPGE.so: olcPGE.cpp olcPixelGameEngine.hpp
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ olcPGE.cpp -I$(INCDIR)

.PHONY+=clean
clean:
	rm -f libolcPGE.so

