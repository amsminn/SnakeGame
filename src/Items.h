#ifndef _ITEMS_H_
#define _ITEMS_H_

#include <utility>
using Pos = std::pair<unsigned int, unsigned int>;

class Items {
private:
    std::vector<Pos> growth;
    std::vector<Pos> poison;
public:
    void addGrowth(Pos pos) { growth.push_back(pos); }
    void addPoison(Pos pos) { poison.push_back(pos); }
    void removeGrowth() { growth.pop_back(); }
    void removePoison() { poison.pop_back(); }
    friend class Game;
};

#endif // _ITEMS_H_