#include "cell.h"


Cell::Cell()
  : isOpen(false)
  , flag(false)
  , value(0)
{

}

Cell::Cell(int value_)
    : isOpen(false)
    , flag(false)
    , value(value_)
{

}



void Cell::toggleFlag()
{
    flag = !flag;
}

void Cell::open()
{
    if (!flag) {
       isOpen = true;
    }
}

void Cell::close()
{
    isOpen = false;
}



int Cell::getValue() const
{
    return value;
}

void Cell::setValue(int value_)
{
    value = value_;
}


bool Cell::isMine() const
{
    return  (value == -1);
}

void Cell::setMine()
{
    value = -1;
}

bool Cell::isFlagged() const
{
    return flag;
}

bool Cell::isOpened() const
{
    return isOpen;
}
