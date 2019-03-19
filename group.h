#ifndef GROUP_H
#define GROUP_H

#include <vector>

#include "bcell.h"
// Група для збережень клітин для методу пошку "Вийняток"
class Group
{
public:
    Group();
    Group(int value_, const BCell &mainCell_);

    BCell mCell();

    bool empty() const;
    int  size() const;
    int  getValue() const;

    void push_back(const BCell &cell);
    void push_back(int x, int y);

    BCell& operator [](const int i);

    Group& operator =(const Group &group_);
    Group  operator -(Group &other);

    bool operator > (Group &group_);
    bool operator < (Group &group_);
    bool operator ==(Group &group_);

private:
    BCell mainCell;
    int value;
    std::vector<BCell> group;
};

#endif // GROUP_H
