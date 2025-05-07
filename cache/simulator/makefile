
JSE:main.o Cache.o Block.o
	g++   -o JSE main.o Cache.o  Block.o -lgflags -std=c++11
Block.o: Block.hpp Block.cpp
	g++ -c Block.cpp  -lgflags -std=c++11


Cache.o:Cache.cpp Cache.hpp Replacement.hpp Block.hpp Block.cpp
	g++ -c  -lgflags Cache.cpp -std=c++11

main.o:simulator.cpp Cache.hpp Replacement.hpp Block.hpp
	g++ -c simulator.cpp  -lgflags -std=c++11 -o main.o

no-cache:
	g++  -o no-cache  no_cache.cpp -lgflags -std=c++11
 
clean:
	rm -f *.o JSE no-cache