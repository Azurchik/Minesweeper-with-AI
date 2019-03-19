#include "valuecell.h"

ValueCell::ValueCell()
    : x (-1)
    , y (-1)
{

}

ValueCell::ValueCell(const int x_, const int y_)
    : x (x_)
    , y (y_)
{

}





void ValueCell::pushBackValue(const float value)
{
    values.push_back(value);
}


float ValueCell::getValue() const
{
    return mainValue;
}



void ValueCell::refineValues() // перезапис значення
{
    if (values.size() == 1) {
        mainValue = values[0];
        values.pop_back();
        return;
    }

    float sum = 1;
    for (float value: values) {
        sum *= 1 - value;
    }

    values.clear();
    mainValue = 1 - sum;
}

void ValueCell::refineValues(float value)
{
    mainValue *= value;
}



bool ValueCell::operator <(const ValueCell &cell)
{
    return mainValue < cell.mainValue;

}

ValueCell& ValueCell::operator =(const ValueCell &cell)
{
    mainValue = cell.mainValue;
    x         = cell.x;
    y         = cell.y;

    return *this;
}



int ValueCell::getX() const
{
    return x;
}

int ValueCell::getY() const
{
    return y;
}
