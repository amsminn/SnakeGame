#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <queue>
#include <utility>
using Pos = std::pair<unsigned int, unsigned int>;

#include "Key.h"

class Snake {
private:
    std::deque<Pos> snake;
    KEY dir;
public:
    Snake() {
        snake.push_back(std::make_pair(15, 15));
        snake.push_back(std::make_pair(15, 14));
        snake.push_back(std::make_pair(15, 13));
        dir = RIGHT;
    }
    void setDir(KEY key) { dir = key; }
    void move(Pos next) {
        snake.push_front(next);
        snake.pop_back();
    }
    void growth(Pos pos) {
        snake.push_front(pos);
    }
    void poison() {
        snake.pop_back();
    }
    [[nodiscard]] Pos getHead() const {
        return snake.front();
    }
    [[nodiscard]] KEY getDir() const { 
        return dir;
    }
    [[nodiscard]] Pos getNext() const {
        const auto [y, x] = snake.front();
        return {y + dy[dir], x + dx[dir]};
    }
    [[nodiscard]] unsigned int length() const { 
        return snake.size();
    }
    friend class Game;
};

#endif // _SNAKE_H_