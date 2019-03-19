#include "bcell.h"

BCell::BCell()
    : x(-1)
    , y(-1)
{

}

BCell::BCell(int x_, int y_)
    : x(x_)
    , y(y_)
{

}




int BCell::getX() const
{
    return x;
}

int BCell::getY() const
{
    return y;
}



bool BCell::equally(int x_, int y_)
{
    return (x_ == x && y_ == y);
}

bool BCell::operator == (const BCell &cell)
{
    return (x == cell.x && y == cell.y);
}

BCell& BCell::operator = (const BCell &cell)
{
    this->x = cell.x;
    this->y = cell.y;
    return *this;
}
