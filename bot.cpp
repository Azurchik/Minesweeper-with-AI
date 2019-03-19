#include "bot.h"

Bot::Bot(Controller *controller_)
{
    controller = controller_; // ініціалізуєм контроллер
    height = controller_->getHeight();
    width  = controller_->getWidth();
}



void Bot::firstStep()
{
    int x = width  / 2;
    int y = height / 2;

    controller->firstStep(x, y);
}



bool Bot::easySearch()
{ // ставить прапорці, коли біля значення стоїть такаж кількість клітин
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int value = controller->getValue(x, y);
            if (value >= 9 || value <= 0)
                continue;

            if (okCell(x, y))
                continue;

            int nine = amountOf9Around(x, y);
            int flag = amountOfFlagsAround(x, y);

            if (nine == (value - flag)) {
                flagedCellsAround(x, y);
                return true;
            }

        }
    }

    return false;
}

bool Bot::exception()
{ // шукає ходи за методом винятки
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int value_ = controller->getValue(x, y);
            if (value_ >= 9 || value_ <= 0)
                continue;

            if (!okCell(x, y)) { // ініціалізуєм першу групу
                Group mainGroup(initializeGroup(x, y));

                std::vector<BCell> list; // записуємо сусідні невідкриті клітини
                findNeighborCells(mainGroup, list);

                for (BCell &cell: list) {
                    Group group(initializeGroup(cell)); // ініціалізуєм другу групу

                    if (findExceptions(group, mainGroup)) { //знаходим винятки
                       return true;
                    } else {
                        continue;
                    }
                }
            }

        }
    }

    return false;
}

void Bot::probability()
{ // знаходимо клітини за методом можливості
    std::vector<Block> list;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int value_ = controller->getValue(x, y);
            if (value_ >= 9 || value_ <= 0)
                continue;

            if (!okCell(x, y)) {
                list.push_back(initializeBlock(x, y));
            }
        }
    }

    for (Block &block: list)
        block.initializeCells();

    for (int i = 0; i < width / 2; ++i) { // поновлюємо точність
        for (Block &block: list)
            block.refineCellValues();
    }

    ValueCell min = list[0].minimumValue();
    for (Block &block: list) { // шукаємо найменшу ймовірність
        if (block.minimumValue() < min)
            min = block.minimumValue();
    }
    controller->openCell(min.getX(), min.getY());
}

bool Bot::lastSteps()
{
    return false;
}

void Bot::openCells()
{ // якщо біля значення стільки ж прапорців - відкриваємо
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int value = controller->getValue(x, y);
            if (value >= 9 || value <= 0)
                continue;

            if (value == amountOfFlagsAround(x, y)) {
                openCellsAround(x, y);
            }
        }
    }
}




bool Bot::findExceptions(Group &group1, Group &group2)
{
    int common = numberCommonlCells(group1, group2); // кількість спільних клітин

    if (group1 == group2 // якщо значення однакові і однакова їх кількість - вихід
            && group1.size() == group2.size()) {
        return false;
    }

    Group temp; // група для результату
    if (common == group1.size()) { // спільні - це всі клітини 1-ї групи
        temp = group2 - group1;
    } else if (common == group2.size()) { // спільні - це всі клітини 2-ї групи
        temp = group1 - group2;
    }
    if (!temp.empty())
        return processGroup(temp);

    if (group1 == group2) // якщо значення однакові - вихід
        return false;

    int value     = group1.getValue();
    int leftCells = group1.size() - common; // неспільні клітини

    if (value < group2.getValue()) { // якщо інша група менше(значенню)
        value     = group2.getValue();
        leftCells = group2.size() - common;
    }

    if (common >= value && leftCells < value) {
        Group temp1 = group1 - group2;
        Group temp2 = group2 - group1;

        bool obj1 = processGroup(temp1);
        bool obj2 = processGroup(temp2);

        return (obj1 || obj2);
    }

    return false;
}




bool Bot::okCell(int x, int y)
{
   return controller->getValue(x, y) == amountOfFlagsAround(x, y);
}




Block Bot::initializeBlock(int x, int y)
{
    int value = controller->getValue(x, y) - amountOfFlagsAround(x, y);
    Block block(value);

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                block.push_back(j, i);
        }
    }
    return block;
}




void Bot::findNeighborCells(Group &group, std::vector<BCell> &listCells)
{
    for (int k = 0; k < group.size(); ++k) {
        int x = group[k].getX();
        int y = group[k].getY();

        for (int i = y - 1; i <= y + 1; ++i) {
            for (int j = x - 1; j <= x + 1; ++j) {
                if (i == y || j == x) {
                    continue;
                }

                if (group.mCell().equally(j, i)) { // якщо рівні головному значенню
                    continue;
                }

                int value = controller->getValue(j, i);
                if (value >= 9 || value <= 0) {
                    continue;
                }

                if (!okCell(j, i)) {
                    listCells.push_back({j, i});
                }
            }
        }
    }
}

Group Bot::initializeGroup(int x, int y)
{
    int value = controller->getValue(x, y) - amountOfFlagsAround(x, y);
    Group group(value, {x, y});

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                group.push_back(j, i);
        }
    }
    return group;
}

Group Bot::initializeGroup(const BCell &cell) //перевизначення методу
{
    int x = cell.getX();
    int y = cell.getY();

    int value = controller->getValue(x, y) - amountOfFlagsAround(x, y);
    Group group(value, cell);

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                group.push_back(j, i);
        }
    }
    return group;
}

int Bot::numberCommonlCells(Group &group1, Group &group2)
{
    int number = 0;
    for (int i = 0; i < group1.size(); ++i) {
        for (int j = 0; j < group2.size(); ++j) {
            if (group1[i] == group2[j]) {
                number++;
                break;
            }
        }
    }
    return number;
}

bool Bot::processGroup(Group &group_)
{
    int value = group_.getValue();
    int size  = group_.size();
    if (value <= 0) {
        for (int i = 0; i < size; ++i) {
            int x = group_[i].getX();
            int y = group_[i].getY();

            controller->openCell(x, y);
        }
        return true;
    }
    if (size == value) {
        for (int i = 0; i < size; ++i) {
            int x = group_[i].getX();
            int y = group_[i].getY();

            controller->toggleFlag(x, y);
        }
        return true;
    }

    return false;
}



void Bot::flagedCellsAround(int x, int y)
{
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                controller->toggleFlag(j, i);
        }
    }
}

void Bot::openCellsAround(int x, int y)
{
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                controller->openCell(j, i);
        }
    }
}




int Bot::amountOf9Around(int x, int y)
{
    int result = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 9)
                ++result;
        }
    }
    return result;
}

int Bot::amountOfFlagsAround(int x, int y)
{
    int result = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j == x && i == y)
               continue;

            if (controller->getValue(j, i) == 10)
                ++result;
        }
    }
    return result;
}




