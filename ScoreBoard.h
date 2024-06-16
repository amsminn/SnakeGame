#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

class ScoreBoard {
private:
    WINDOW *scoreBoard;
    unsigned int nowLength, mxLength, P, M, G;
public:
    void draw(unsigned int tick) {
        wclear(scoreBoard);
        box(scoreBoard, 0, 0);
        mvwprintw(scoreBoard, 1, 1, "SCORE BOARD");
        mvwprintw(scoreBoard, 3, 1, "B : %u/%u", nowLength, mxLength);
        mvwprintw(scoreBoard, 5, 1, "+ : %u", P);
        mvwprintw(scoreBoard, 7, 1, "- : %u", M);
        mvwprintw(scoreBoard, 9, 1, "G : %u", G);
        mvwprintw(scoreBoard, 11, 1, "S : %u", tick == 250 ? 2 : 1);
        wrefresh(scoreBoard);
        curs_set(0);
    }
    ScoreBoard(): nowLength(3), mxLength(3), P(0), M(0), G(0) {
        int height, width;
        getmaxyx(stdscr, height, width);
        scoreBoard = subwin(stdscr, 14, 30, 3, 62);

	    draw(500);
    }
    void setNowLength(unsigned int nowLength) { 
        this->nowLength = nowLength; 
        if(nowLength > mxLength) mxLength = nowLength;
    }
    void incP() { P++; }
    void incM() { M++; }
    void incG() { G++; }
};

#endif // _SCOREBOARD_H_