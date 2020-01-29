all: sfml-app

sfml-app: main.o
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.o: main.cpp
	g++ -c main.cpp