
JSE:main.o Cache.o Block.o
	g++ -o JSE main.o Cache.o  Block.o -std=c++11
Block.o: Block.hpp Block.cpp
	g++ -c Block.cpp -std=c++11


Cache.o:Cache.cpp Cache.hpp Replacement.hpp Block.hpp Block.cpp
	g++ -c Cache.cpp -std=c++11

main.o:simulator.cpp Cache.hpp Replacement.hpp Block.hpp
	g++ -c simulator.cpp -std=c++11 -o main.o
 
clean:
	rm -f *.o JSE