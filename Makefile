all:
	g++ -I src/include/single_include -L src/lib -o main main.cpp DemoGame.cpp Game.cpp Scene.cpp Entity.cpp -lSDL2 -lSDL2main -lSDL2_ttf
