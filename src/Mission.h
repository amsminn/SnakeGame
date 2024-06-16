#ifndef _MISSION_H_
#define _MISSION_H_

#include <cassert>
#include <algorithm>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

class Mission {
private:
    unsigned int B, P, M, G;
    unsigned int b, p, m, g;
    WINDOW *mission;
public:
    void addB(unsigned int _B) {
        b = std::max(b, _B);
        draw();
    }
    void addP() {
        p++;
        draw();
    }
    void addM() {
        m++;
        draw();
    }
    void addG() {
        g++;
        draw();
    }
    [[nodiscard]] char toChar(bool a) const { return a ? 'v' : ' '; }
    void draw() {
        wclear(mission);
        box(mission, 0, 0);

        mvwprintw(mission, 1, 1, "MISSION");
        mvwprintw(mission, 3, 1, "B : %u (%c)", B, toChar(b >= B));
        mvwprintw(mission, 5, 1, "+ : %u (%c)", P, toChar(p >= P));
        mvwprintw(mission, 7, 1, "- : %u (%c)", M, toChar(m >= M));
        mvwprintw(mission, 9, 1, "G : %u (%c)", G, toChar(g >= G));
        
        wrefresh(mission);
        curs_set(0);
    }
    [[nodiscard]] bool missionClear() const { return b >= B and p >= P and m >= M and g >= G; }
    Mission(unsigned int B, unsigned int P, unsigned int M, unsigned int G, unsigned int _B = 3, unsigned int _P = 0, unsigned int _M = 0, unsigned int _G = 0): 
    B(B), P(P), M(M), G(G), b(_B), p(_P), m(_M), g(_G) {
        int height, width;
        getmaxyx(stdscr, height, width);
        mission = subwin(stdscr, 12, 30, 21, 62);

	    draw();
    }
};

#endif // _MISSION_H_