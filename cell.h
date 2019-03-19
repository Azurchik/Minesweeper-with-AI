#ifndef CELL_H
#define CELL_H
// Клітини для заповнення поля гри
class Cell
{
public:
    Cell();
    Cell(int value_);

    void setValue(int value);
    void setMine();
    int  getValue() const;

    bool isFlagged() const;
    bool isMine()    const;
    bool isOpened()  const;

    void open();
    void close();
    void toggleFlag();


private:
    bool isOpen;
    bool flag;
    int  value;
};

#endif // CELL_H
