#include <iostream>
#include <chrono>
#include <thread>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

#include "Game.h"

int main() {
	srand(time(NULL));

	initscr(); 
	start_color();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);

	initColorPair();
	attron(COLOR_PAIR(GLOBAL_TEXT));
	bkgd(COLOR_PAIR(GLOBAL_TEXT));
	attroff(COLOR_PAIR(GLOBAL_TEXT)); 

	#define HEIGHT 34
	#define WIDTH 94
	resize_term(HEIGHT, WIDTH);	
	box(stdscr, 0, 0);
	mvprintw(1, 1, "SNAKE GAME"); 
	curs_set(0); 

	// int B[5] = {0, 3, 5, 7, 9};
	// int P[5] = {0, 1, 2, 3, 4};
	// int M[5] = {0, 1, 2, 3, 4};
	// int G[5] = {0, 1, 2, 3, 4};

	int B[5] = {0, 3, 4, 5, 6};
	int P[5] = {0, 2, 3, 4, 5};
	int M[5] = {0, 1, 1, 2, 2};
	int G[5] = {0, 1, 1, 2, 2};

	auto st = std::chrono::system_clock::now();
	int tick = 500;
	for(unsigned int round = 2; round <= 4; round++) {
		Game game(round, B[round], P[round], M[round], G[round]);

		char ch = 'd'; // right direction 
		unsigned int count = 0;

		int inGate = 0;
		int prevLength = 3;
		int tickCount = 10;
		while(!game.isOver()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(tick));
			if(tick == 250) tickCount--;
			if(tickCount == 0) {
				tick = 500;
			}
			char ch = getch();
			switch(ch) {
				case 'w':
					game.move(UP);
					break;
				case 's':
					game.move(DOWN);
					break;
				case 'a':
					game.move(LEFT);
					break;
				case 'd':
					game.move(RIGHT);
					break;
				case ERR:
					game.move(game.getDir());
					break;
				default: 
					game.gameOver();
					break;
			}
			if(game.isOver()) goto END;
			if(game.missionClear()) break;
			if(count % 30 == 5) {
				game.removeGrowth();
				game.addGrowth();
			}
			if(count % 30 == 15) {
				game.removePoison();
				game.addPoison();
			}
			if(count % 30 == 25) {
				game.clearGate();
				game.addGate();
			}
			if(count % 30 == 11) {
				game.removeSpeed();
				game.addSpeed();
			}
			if(count % 30 == 20) {
				game.closePassWalls();
				game.genPassWalls();
			}
			game.print((unsigned int)tick);
			auto et = std::chrono::system_clock::now();
			mvwprintw(stdscr, 1, 70, "PLAYTIME : %03dseconds", (int)std::chrono::duration_cast<std::chrono::seconds>(et - st).count());
			if(game.gateCollision()) {
				inGate = game.getLength();
			}
			if(game.speedCollision()) {
				game.removeSpeed();
				tickCount = 20;
				tick = 250;
			}
			if(prevLength < game.getLength()) {
				tickCount = 20;
				tick = 250;
			}
			prevLength = game.getLength();
			if(!inGate) count = (count + 1) % 30;
			else inGate--;
		}

		if(round == 4 and game.missionClear()) {
			game.gameClear();
			goto END;
		}
	}

	END:

	refresh(); 
  	endwin();

	return 0;
}