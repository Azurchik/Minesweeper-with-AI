#ifndef VALUECELL_H
#define VALUECELL_H

#include <vector>

// Вид клітин для методи пошуку "Ймовірність".
// Має головне значення і має методи для визначення цього значення.
class ValueCell
{
public:
    ValueCell();
    ValueCell(const int x_, const int y_);

    void pushBackValue(const float value);

    float getValue() const;

    void refineValues();
    void refineValues(float value);

    bool operator <(const ValueCell &cell);

    ValueCell& operator =(const ValueCell &cell);

    int getX() const;
    int getY() const;

private:
    int x;
    int y;

    float mainValue;
    std::vector<float> values;
};

#endif // VALUECELL_H
