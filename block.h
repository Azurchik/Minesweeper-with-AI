#ifndef BLOCK_H
#define BLOCK_H

#include "valuecell.h"

#include <vector>
//Спосіб збереження клітин для методу "Ймовірність"
class Block
{
public:
    Block();
    Block(float value_);

    void initializeCells();
    void refineCellValues();

    ValueCell minimumValue();

    void push_back(int x, int y);
    void push_back(const ValueCell &cell);

    ValueCell& operator [](const unsigned int i);

private:
    bool checkRepeat;
    float value;
    std::vector<ValueCell> list;
};

#endif // BLOCK_H
