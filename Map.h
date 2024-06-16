#ifndef _MAP_H_
#define _MAP_H_

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <string>
#include <sstream> 
#include "Cell.h"

#define N 30
#define M 30

class Map {
private:
    int round;
    TYPE map[N][M]; // N x M, board[y][x]
    WINDOW *win;
public:
    void initMap() {
        map[0][0] = map[0][M - 1] = map[N - 1][0] = map[N - 1][M - 1] = IMMUNE;
        for (unsigned int y = 1; y + 1 < N; y++) {
            map[y][0] = map[y][M - 1] = WALL;
        }
        for (unsigned int x = 1; x + 1 < M; x++) {
            map[0][x] = map[N - 1][x] = WALL;
        }
        for (unsigned int y = 1; y + 1 < N; y++) {
            for (unsigned int x = 1; x + 1 < M; x++) {
                map[y][x] = EMPTY;
            }
        }
        for (unsigned int y = 17; y < 25; y++) map[y][24] = WALL;
        for (unsigned int x = 15; x < 25; x++) map[24][x] = WALL;
        for (unsigned int y = 5; y < 15; y++) map[y][5] = WALL;
    }
    void fill1() {
        for (unsigned int y = 17; y < 25; y++) map[y][24] = WALL;
        for (unsigned int x = 15; x < 25; x++) map[24][x] = WALL;
        for (unsigned int y = 5; y < 15; y++) map[y][5] = WALL;
    }
    void fill2() {
        for (unsigned int x = 5; x < 13; x++) map[5][x] = WALL;
        for (unsigned int x = 17; x < 25; x++) map[17][x] = WALL;
        for (unsigned int x = 7; x < 15; x++) map[25][x] = WALL;
    }
    void fill3() {
        for (unsigned int y = 5; y < 13; y++) map[y][5] = WALL;
        for (unsigned int y = 17; y < 25; y++) map[y][17] = WALL;
        for (unsigned int y = 7; y < 15; y++) map[y][25] = WALL;
        for (unsigned int x = 7; x < 15; x++) map[18][x] = WALL;
    }
    void fill4() {
        for (unsigned int x = 5; x < 25; x++) map[5][x] = WALL;
        for (unsigned int x = 5; x < 25; x++) map[18][x] = WALL;
        for (unsigned int x = 5; x < 25; x++) map[25][x] = WALL;
        for (unsigned int y = 5; y < 25; y++) map[y][3] = WALL;
    }
    Map(int round): round(round) {
        win = subwin(stdscr, 30, 60, 3, 1);
        initMap();
        switch(round) {
            case 1:
                fill1(); 
                break;
            case 2:
                fill2();
                break;
            case 3:
                fill3();
                break;
            case 4:
                fill4();
                break;
            default:
                break;
        }
        draw(2);
    }
    [[nodiscard]] TYPE getType(unsigned int y, unsigned int x) const {
        return map[y][x];
    }
    void setType(unsigned int y, unsigned int x, TYPE type) {
        map[y][x] = type;
    }
    void draw(int isOver = 0) const {
        wclear(win);
        wbkgd(win, COLOR_PAIR(EMPTY));

        for(unsigned int y = 0; y < N; y++) {
            for(unsigned int x = 0; x < M; x++) {
                // fill the map[y][x] cell with color_pair(map[y][x])
#ifdef _WIN32
                mvwaddch(win, y, 2 * x, mapColorToWin32FallbackText(map[y][x]));
                mvwaddch(win, y, 2 * x + 1, mapColorToWin32FallbackText(map[y][x]));
#else
                wattron(win, COLOR_PAIR(map[y][x]));
                mvwaddch(win, y, 2 * x, ' ');
                mvwaddch(win, y, 2 * x + 1, ' ');
                wattroff(win, COLOR_PAIR(map[y][x]));
#endif
            }
        }

        if(isOver == 1) {
            std::string str = "GAME OVER";
#ifndef _WIN32
            wattron(win, COLOR_PAIR(CENTER_TEXT));
#endif
            for(unsigned int i = 0; i < str.size(); i++) {
                mvwprintw(win, 13, 25 + i, "%c", str[i]);
            }
#ifndef _WIN32
            wattroff(win, COLOR_PAIR(CENTER_TEXT));
#endif
        }

        if(isOver == 2) {
            std::string str = " ROUND " + std::to_string(round) + " "; 
#ifndef _WIN32
            wattron(win, COLOR_PAIR(CENTER_TEXT));
#endif
            for(unsigned int i = 0; i < str.size(); i++) {
                mvwprintw(win, 13, 25 + i, "%c", str[i]);
            }
#ifndef _WIN32
            wattroff(win, COLOR_PAIR(CENTER_TEXT));
#endif
        }

        if(isOver == 3) {
            std::string str = "GAME CLEAR";
#ifndef _WIN32
            wattron(win, COLOR_PAIR(CENTER_TEXT));
#endif
            for(unsigned int i = 0; i < str.size(); i++) {
                mvwprintw(win, 13, 25 + i, "%c", str[i]);
            }
#ifndef _WIN32
            wattroff(win, COLOR_PAIR(CENTER_TEXT));
#endif
        }

        wrefresh(win);
    }

    void gameOver() {
        for(unsigned int y = 0; y < N; y++) {
            for(unsigned int x = 0; x < M; x++) {
                map[y][x] = WALL;
            }
        }
        draw(1);
    }

    void gameClear() {
        for(unsigned int y = 0; y < N; y++) {
            for(unsigned int x = 0; x < M; x++) {
                map[y][x] = WALL;
            }
        }
        draw(3);
    }
};

#undef N
#undef M
#endif // _MAP_H_