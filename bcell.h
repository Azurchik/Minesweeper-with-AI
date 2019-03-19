#ifndef BCELL_H
#define BCELL_H

//Клітини для методу "Вийняток"
class BCell
{
public:
    BCell();
    BCell(int x_, int y_);

    int getX() const;
    int getY() const;

    bool equally(int x_, int y_);
    bool operator ==(const BCell &cell);

    BCell& operator =(const BCell &cell);

protected:
    int x;
    int y;
};

#endif // BCELL_H
