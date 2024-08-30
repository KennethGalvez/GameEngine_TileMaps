all:
	g++ -I src/include/single_include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2
