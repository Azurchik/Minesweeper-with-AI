#include "controller.h"

#include <queue>
#include <QtDebug>

#include "bcell.h"

Controller::Controller(Area *area_)
    : flagUsed(0)
    , mistake (false)
    , area (area_)
{
}



void Controller::firstStep(int x, int y)
{
    area->initialize(x, y);
    openCell(x, y);
}

int Controller::getHeight()
{
    return area->getHeight();
}

int Controller::getWidth()
{
    return area->getWidth();
}



int Controller::getValue(int x, int y)
{
    if (area->at(x, y).isFlagged())
        return 10;

    if (!area->at(x, y).isOpened())
        return 9;

    return area->at(x, y).getValue();
}



void Controller::openCell(int x, int y)
{
    Cell &cell = area->at(x, y);
    if (cell.isFlagged())
        return;

    cell.open();

    if (cell.isMine()) {
        mistake = true;
    } else if (cell.getValue() == 0) {
        openAllEmptyAround(x, y);
    }
}

void Controller::openCellsAround(int x, int y)
{ // якщо кількість прапрорів біля значення співпадає зі значенням,
    // відкриваються всі інші клітини біля значення
    int val = getValue(x, y);
    if (val == 0 || val == 10 || val == 9)
        return;

    int flags = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (area->at(j, i).isFlagged())
                ++flags;
        }
    }

    if (val == flags) {
        for (int i = y - 1; i <= y + 1; ++i) {
            for (int j = x - 1; j <= x + 1; ++j) {
                if (i == y && j == x)
                    continue;

                openCell(j, i);
            }
        }
    }

}

void Controller::openAllEmptyAround(int x_, int y_) // Open cells around zero cells.
{
    std::queue<BCell>  useCells;
    std::vector<BCell> usedCells;

    useCells.push({x_, y_});
    do {
        int y = useCells.front().getY();
        int x = useCells.front().getX();

        for (int i = y - 1; i <= y + 1; i++) {
            for (int j = x - 1; j <= x + 1; j++) {
                if (j == x && i == y)
                   continue;

                int value = area->at(j, i).getValue();

                if (value == -2) // якщо координати поза межі поля
                    continue;

                if (value == 0) { // записуємо клітини з нулями для перевірки
                    if (usedCells.empty()) {
                        useCells.push({j, i});

                    } else { // перевіряємо не повторення клітин
                        bool detected = false;

                        for (BCell &bCell: usedCells) {
                            if (bCell.equally(j, i)) {
                                detected = true;
                                break;
                            }
                        }
                        if (!detected)
                            useCells.push({j, i});
                    }
                }
                area->at(j, i).open();
            }
        }
        usedCells.push_back({x, y}); // переміщаєм використанні клітини в інший список
        useCells.pop(); // і видиляєм з теперішнього

    } while (!useCells.empty());
}



void Controller::toggleFlag(int x, int y)
{
    if (area->at(x, y).isOpened())
        return;

    if (area->at(x, y).isFlagged()) { // лічильник прапорів на полі
        flagUsed--;
    } else {
        ++flagUsed;
    }
    area->at(x, y).toggleFlag();
}



bool Controller::gameWon()
{ // якщо всі закриті клітини міни - гра виграна
    for (int y = 0; y < area->getHeight(); y++) {
        for (int x = 0; x < area->getWidth(); x++) {
            Cell cell = area->at(x, y);

            if (cell.isOpened()) {
                if (cell.isMine())
                    return false;
            } else {
                if (!cell.isMine())
                    return false;
            }
        }
    }

    return true;
}

bool Controller::gameLost()
{
    return mistake;
}



int Controller::minesLeft()
{
    return area->getNumberMines() - flagUsed;
}

void Controller::reload()
{
    mistake = false;
    flagUsed = 0;
}
