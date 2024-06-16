#ifndef _KEY_H_
#define _KEY_H_

enum KEY {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

// up, right, down, left
const int dy[4] = {-1, 0, 1, 0};
const int dx[4] = {0, 1, 0, -1};

#endif // _KEY_H_