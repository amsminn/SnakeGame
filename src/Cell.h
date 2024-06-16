#ifndef _CELL_H_
#define _CELL_H_

#ifdef _WIN32
#include <ncurses/ncurses.h>
#include <windows.h>
#else
#include <ncurses.h>
#endif

enum TYPE {
    WALL = 30,
    IMMUNE,
    HEAD,
    BODY,
    GATE,
    GROWTH,
    POISON,
    EMPTY,
    CENTER_TEXT,
    CENTER_TEXT_BACKGROUND,
    GLOBAL_TEXT,
    GLOBAL_TEXT_CKGROUND,
    SPEED_ITEM
};

void initColorPair() {
    // wall = gray
    init_color(WALL, 500, 500, 500);  
    init_pair(WALL, WALL, WALL);

    // immune = soft gray
    init_color(IMMUNE, 800, 800, 800); 
    init_pair(IMMUNE, IMMUNE, IMMUNE);

    // head = yellow
    init_color(HEAD, 1000, 1000, 0);
    init_pair(HEAD, HEAD, HEAD);

    // body = orange
    init_color(BODY, 1000, 500, 0);
    init_pair(BODY, BODY, BODY);

    // gate = purple
    init_color(GATE, 1000, 0, 1000);
    init_pair(GATE, GATE, GATE);

    // growth = green
    init_color(GROWTH, 0, 1000, 0);
    init_pair(GROWTH, GROWTH, GROWTH);

    // poison = red
    init_color(POISON, 1000, 0, 0); 
    init_pair(POISON, POISON, POISON);

    // empty = black
    init_color(EMPTY, 0, 0, 0);
    init_pair(EMPTY, EMPTY, EMPTY);

    // Center text
    init_color(CENTER_TEXT, 1000, 0, 0);
    init_color(CENTER_TEXT_BACKGROUND, 1000, 1000, 1000);
    init_pair(CENTER_TEXT, CENTER_TEXT, CENTER_TEXT_BACKGROUND);

    // Global text
    init_color(GLOBAL_TEXT, 1000, 1000, 1000);
	init_color(GLOBAL_TEXT_CKGROUND, 0, 0, 0);
	init_pair(GLOBAL_TEXT, GLOBAL_TEXT, GLOBAL_TEXT_CKGROUND);

    // Speed item
    init_color(SPEED_ITEM, 0, 847, 1000);
    init_pair(SPEED_ITEM, SPEED_ITEM, SPEED_ITEM);
}
#ifdef _WIN32

char mapColorToWin32FallbackText(TYPE color) {
    switch(color) {
        case WALL: return '#';
        case IMMUNE: return 'I';
        case HEAD: return 'H';
        case BODY: return 'B';
        case GATE: return 'G';
        case GROWTH: return '+';
        case POISON: return '-';
        case EMPTY: return ' ';
        case SPEED_ITEM: return 'S';
        default: return ' ';
            break;
    }
}
#endif

#endif // _CELL_H_