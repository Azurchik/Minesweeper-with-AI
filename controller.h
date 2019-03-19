#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "area.h"
// Пристрій для передачі стану поля ігрокам
class Controller
{
public:
    Controller (Area *area_);

    void firstStep(int x, int y);
    int  getWidth();
    int  getHeight();

    int  getValue(int x, int y);

    void openCell(int x, int y);
    void openCellsAround(int x, int y);
    void toggleFlag(int x, int y);

    bool gameWon();
    bool gameLost();

    int minesLeft();
    void reload();

private:
    void openAllEmptyAround(int x_, int y_);

    int  flagUsed;
    bool mistake;

    Area   *area;

};

#endif // CONTROLLER_H
