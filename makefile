CC =g++
EXEC=dameo
OBJ=obj/board.o obj/game.o obj/min_max.o  obj/display2d.o obj/main.o

all: $(OBJ)
	$(CC) $^ -o $(EXEC) -L ./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

obj/main.o : src/main.cpp
	$(CC) -c src/main.cpp -std=c++14 -o $@

obj/display2d.o : src/display2d.cpp
	$(CC) -c src/display2d.cpp -std=c++14 ./SFML/include/ -o $@

obj/min_max.o : src/min_max.cpp
	$(CC) -c src/min_max.cpp -std=c++14 -o $@

obj/game.o : src/game.cpp
	$(CC) -c src/game.cpp -std=c++14 -o $@

obj/board.o : src/board.cpp
	$(CC) -c src/board.cpp -std=c++14 -o $@	

clean:
	rm -rf obj/board.o obj/game.o obj/min_max.o  obj/display2d.o obj/main.o dameo