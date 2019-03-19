#include "group.h"

Group::Group(int value_, const BCell &mainCell_)
    : mainCell(mainCell_)
    , value(value_)
{

}



Group::Group()
    : value(0)
{

}

BCell Group::mCell() // встановлюємо головну клітину
{
    BCell cell = mainCell;
    return cell;
}


bool Group::empty() const
{
    return group.empty();
}

int Group::size() const
{
    return group.size();
}

int Group::getValue() const
{
    return value;
}

void Group::push_back(const BCell &cell)
{
    group.push_back(cell);
}

void Group::push_back(int x, int y)
{
    group.push_back({x, y});
}



BCell& Group::operator [](const int i)
{
    return group[i]; // доступ до клітини
}


Group& Group::operator =(const Group &group_)
{
    value    = group_.value;
    group    = group_.group;
    mainCell = group_.mainCell;
    return *this;
}

bool Group::operator <(Group &group_)
{
    return (value < group_.value);
}

bool Group::operator >(Group &group_)
{
    return (value > group_.value);
}

bool Group::operator ==(Group &group_)
{
    return (value == group_.value);
}

Group Group::operator -(Group &other)
{ // віднімаються значення
    Group temp(value - other.value, mainCell);
    for (BCell &tCell: group) {
        bool detected = false;
        for (BCell &oCell: other.group) { // спільні клітини віднімаються
            if (tCell == oCell) {
                detected = true;
                break;
            }
        } // всі інші залишаються
        if (!detected) {
            temp.push_back(tCell);
        }
    }
    return temp;
}
