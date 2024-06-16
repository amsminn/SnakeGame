#ifndef _GAME_H_
#define _GAME_H_

#include "Map.h"
#include "Snake.h"
#include "Items.h"
#include "Key.h"
#include "ScoreBoard.h"
#include "Mission.h"
#include <cassert>
#include <utility>
#include <map>
#include <thread>

#define HEIGHT 34
#define WIDTH 94

using Pos = std::pair<unsigned int, unsigned int>;

class Game {
private:
    bool isGameOver;
    Map map;
    Snake snake;
    Items items;
    std::map<Pos, Pos> gateMap;
    ScoreBoard scoreBoard;
    Mission mission;
    Pos speedCell;
    std::pair<Pos, Pos> passWalls;
public:
    Game(int round, int B, int P, int M, int G): isGameOver(false), map(round), snake(), items(), scoreBoard(), mission(B, P, M, G) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        passWalls = {{1, 0}, {1, 0}};
        print(500); // update and draw map
    }

    void addSpeed() {
        auto empty = emptyCells();
        if(empty.empty()) return;
        auto pos = empty[rand() % empty.size()];
        map.setType(pos.first, pos.second, SPEED_ITEM);
        speedCell = pos;
    }

    [[nodiscard]] bool speedCollision() const {
        Pos head = snake.getHead();
        return head == speedCell;
    }

    void removeSpeed() {
        if(speedCell != Pos(0, 0)) map.setType(speedCell.first, speedCell.second, EMPTY);
        speedCell = {0, 0};
    }

    void gameOver() { 
        isGameOver = true; 
        map.gameOver();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    void gameClear() {
        map.gameClear();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    std::vector<Pos> innerWallCells() {
        std::vector<Pos> wall;
        for(unsigned int y = 1; y + 1 < 30; y++) {
            for(unsigned int x = 1; x + 1 < 30; x++) {
                if(map.getType(y, x) == WALL) {
                    wall.push_back({y, x});
                }
            }
        }
        return wall;
    }

    void genPassWalls() {
        auto wall = innerWallCells();
        assert(wall.size() >= 2);
        auto pos1 = wall[rand() % wall.size()];
        auto pos2 = wall[rand() % wall.size()];
        while(pos1 == pos2) {
            pos2 = wall[rand() % wall.size()];
        }
        map.setType(pos1.first, pos1.second, EMPTY);
        map.setType(pos2.first, pos2.second, EMPTY);
        passWalls = {pos1, pos2};
    }

    void closePassWalls() {
        auto [pos1, pos2] = passWalls;
        map.setType(pos1.first, pos1.second, WALL);
        map.setType(pos2.first, pos2.second, WALL);
        passWalls = {{1, 0}, {1, 0}};
    }

    [[nodiscard]] bool gateCollision() const {
        Pos head = snake.getHead();
        head.first -= dy[snake.getDir()];
        head.second -= dx[snake.getDir()];
        return map.getType(head.first, head.second) == GATE;
    }

    [[nodiscard]] unsigned int getLength() const {
        return snake.length();
    }

    void move(KEY key) {
        for(const auto &[y, x] : snake.snake) {
            map.setType(y, x, EMPTY);
        }

        if(key == (snake.getDir() + 2) % 4) {
            gameOver();
            return;
        }

        snake.setDir(key);
        Pos next = snake.getNext();

        if(map.getType(next.first, next.second) == WALL or map.getType(next.first, next.second) == BODY) {
            gameOver();
            return;
        }

        auto safe = [&](unsigned int i, unsigned int j) {
            bool ret = 1;
            ret &= 0 <= i and i < 30 and 0 <= j and j < 30;
            if(ret) {
                TYPE type = map.getType(i, j);
                ret &= type == EMPTY or type == GROWTH or type == POISON;
            }
            return ret;
        };

        if(map.getType(next.first, next.second) == GATE) {
            next = gateMap[next];
            int dKey[4] = {0, 1, 3, 2};
            int i = 0;
            while(!safe(next.first + dy[key], next.second + dx[key])) {
                key = (KEY)(((unsigned int)key + dKey[i]) % 4);
                i++;
            }
            next = {next.first + dy[key], next.second + dx[key]};
            snake.setDir(key);
            mission.addG();
            scoreBoard.incG();
        }

        if(map.getType(next.first, next.second) == GROWTH) {
            snake.growth(next);
            map.setType(items.growth.back().first, items.growth.back().second, BODY);
            scoreBoard.incP();
            scoreBoard.setNowLength(snake.length());
            mission.addP();
            mission.addB(snake.length());
            items.removeGrowth();
            return;
        }
        
        if(map.getType(next.first, next.second) == POISON) {
            snake.poison();
            map.setType(items.poison.back().first, items.poison.back().second, EMPTY);
            scoreBoard.incM();
            scoreBoard.setNowLength(snake.length());
            mission.addM();
            items.removePoison();
            return;
        }

        // EMPTY or ITEM
        snake.move(next);

        if(snake.length() < 3) {
            gameOver();
            return;
        }
    }

    void updateMap() {
        // map.initMap();

        map.setType(snake.getHead().first, snake.getHead().second, HEAD);
        for(auto it = snake.snake.begin() + 1; it != snake.snake.end(); it++) {
            map.setType(it->first, it->second, BODY);
        }

        for(const auto &[y, x]: items.growth) {
            map.setType(y, x, GROWTH);
        }

        for(const auto &[y, x]: items.poison) {
            map.setType(y, x, POISON);
        }
    }

    std::vector<Pos> emptyCells() {
        std::vector<Pos> empty;
        for(unsigned int y = 1; y + 1 < 30; y++) {
            for(unsigned int x = 1; x + 1 < 30; x++) {
                if(map.getType(y, x) == EMPTY) {
                    empty.push_back({y, x});
                }
            }
        }
        return empty;
    }

    std::vector<Pos> wallCells() {
        std::vector<Pos> wall;
        for(unsigned int y = 0; y < 30; y++) {
            for(unsigned int x = 0; x < 30; x++) {
                if(map.getType(y, x) == WALL) {
                    wall.push_back({y, x});
                }
            }
        }
        return wall;
    }

    void addGate() {
        auto wall = wallCells();
        assert(wall.size() >= 2);
        auto pos1 = wall[rand() % wall.size()];
        auto pos2 = wall[rand() % wall.size()];
        while(pos1 == pos2) {
            pos2 = wall[rand() % wall.size()];
        }
        map.setType(pos1.first, pos1.second, GATE);
        map.setType(pos2.first, pos2.second, GATE);
        gateMap[pos1] = pos2;
        gateMap[pos2] = pos1;
    }

    void clearGate() {
        for(const auto &[pos1, pos2]: gateMap) {
            map.setType(pos1.first, pos1.second, WALL);
            map.setType(pos2.first, pos2.second, WALL);
        }
        gateMap.clear();
    }

    void addGrowth() {
        auto empty = emptyCells();
        if(empty.empty()) {
            gameOver();
            return;
        }
        auto pos = empty[rand() % empty.size()];
        map.setType(pos.first, pos.second, GROWTH);
        items.addGrowth(pos);
    }

    void removeGrowth() {
        if(items.growth.empty()) return;
        map.setType(items.growth.back().first, items.growth.back().second, EMPTY);
        items.removeGrowth();
    }

    void addPoison() {
        auto empty = emptyCells();
        if(empty.empty()) {
            gameOver();
            return;
        }
        auto pos = empty[rand() % empty.size()];
        map.setType(pos.first, pos.second, POISON);
        items.addPoison(pos);
    }

    void removePoison() {
        if(items.poison.empty()) return;
        map.setType(items.poison.back().first, items.poison.back().second, EMPTY);
        items.removePoison();
    }

    void print(unsigned int tick) {
        updateMap();
        scoreBoard.draw(tick);
        mission.draw();
        map.draw();
    }

    [[nodiscard]] bool isOver() const {
        return isGameOver;
    }

    [[nodiscard]] KEY getDir() const { return snake.getDir(); }

    [[nodiscard]] bool missionClear() const {
        return mission.missionClear();
    }
};

#endif // _GAME_H_