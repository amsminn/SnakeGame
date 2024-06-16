main: ./src/main.cpp
	g++ -o main ./src/main.cpp -std=c++17 -lncurses -DNCURSES_STATIC 

clean:
	rm -f main

.PHONY: clean