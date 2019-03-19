#ifndef BOT_H
#define BOT_H

#include "controller.h"
#include "block.h"
#include "group.h"
// Робот для вирішення гри
class Bot
{
public:
    Bot(Controller *controller_);

    void firstStep();

    bool easySearch();
    bool exception();
    bool lastSteps();
    void probability();
    void openCells();

private:    
    bool findExceptions(Group &group1, Group &group2);

    bool okCell(int x, int y);

    Block initializeBlock(int x, int y);

    void  findNeighborCells(Group &group, std::vector<BCell> &listCells);
    Group initializeGroup(int x, int y);
    Group initializeGroup(const BCell &cell);
    int   numberCommonlCells(Group &group1, Group &group2);
    bool  processGroup(Group &group_);

    void flagedCellsAround(int x, int y);
    void openCellsAround(int x, int y);

    int amountOf9Around(int x, int y);
    int amountOfFlagsAround(int x, int y);

    Controller *controller;
    int width;
    int height;

};

#endif // BOT_H
