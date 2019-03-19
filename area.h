#ifndef AREA_H
#define AREA_H

#include<vector>

#include "cell.h"
// Поле гри зі значеннями клітин
class Area
{
public:
    Area(int width, int height, int numberMines);

    int  getWidth() const;
    int  getHeight() const;
    int  getNumberMines() const;

    void initialize(int clickX, int clickY);

    Cell& at(int x, int y);

private:
    void calculateNumberOfMinesAround(int x, int y);

    int width;
    int height;
    int numberOfMines;

    Cell stub;

    std::vector<std::vector<Cell>> gameArea;

};

#endif // AREA_H
