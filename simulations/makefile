CC=g++ -std=c++17
LIBS=-lm -lX11 -lGL -lpng16 -Wl,-rpath,. -L. -lolcLib

main: main.cpp bounce.cpp libolcLib.so
	$(CC) -o $@ main.cpp $(LIBS)

libolcLib.so: olcLib.cpp ../olcpixeng/include/olcPixelGameEngine.h
	$(CC) -shared -fPIC -o $@ olcLib.cpp

clean:
	rm -f main libolcLib.so

