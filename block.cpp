#include "block.h"

Block::Block()
    : checkRepeat(false)
{

}

Block::Block(float value_)
    : value(value_)
    , checkRepeat (false)
{

}



void Block::initializeCells()
{
    int size = static_cast<int>(list.size());
    for (ValueCell &cell: list) {
        cell.pushBackValue(value / size); // записуєм початкове значення у клітинни
    }
}

void Block::refineCellValues()
{
    if (!checkRepeat) {
        for (ValueCell &cell: list) { // перезаписуємо значення
            cell.refineValues();
        }
        checkRepeat = true;
    }

    float sum = 0;
    for (ValueCell &cell: list) { // сума всіх значень
        sum += cell.getValue();
    }

    for (ValueCell &cell: list) { // поновлюємо точність
        cell.refineValues(value / sum);
    }
}



ValueCell Block::minimumValue()
{
    ValueCell min = list[0];

    for (ValueCell &cell: list) {
        if (cell < min) {
            min = cell;
        }
    }

    return min;
}



void Block::push_back(const ValueCell &cell)
{
    list.push_back(cell);
}

void Block::push_back(int x, int y)
{
    list.push_back({x, y});
}



ValueCell& Block::operator[](const unsigned int i)
{
    return list[i];
}
