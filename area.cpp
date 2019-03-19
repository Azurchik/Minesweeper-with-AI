#include "area.h"

#include <QtGlobal>
#include <QTime>



Area::Area(int width_, int height_, int numberMines_)
  : width (width_)
  , height(height_)
  , numberOfMines(numberMines_)
  , stub(-2) // ініціалізація клітини для поза межного звертання
{
    for (int i = 0; i < height; i++) { // заповнення поля пустими клітинами
        gameArea.push_back({});
        for (int j = 0; j < width; j++) {
            gameArea[i].push_back({});
        }
    }

    QTime time = QTime::currentTime(); // налаштування для рандому
    srand(static_cast<uint>(time.msec()));
}



void Area::initialize(int clickX, int clickY)
{
    for (int i = 0; i < numberOfMines; i++) { //розміщаємо міни
      int mineX;
      int mineY;
      do {
        mineX = rand() % width;
        mineY = rand() % height;
      } while ((mineX >= (clickX - 1) && mineX <= (clickX + 1)
                && mineY >= (clickY - 1) && mineY <= (clickY + 1))
               || gameArea[mineY][mineX].isMine());

      gameArea[mineY][mineX].setMine();
    }

    for (int y = 0; y < height; y++) { // ставо значення клітин
      for (int x = 0; x < width; x++) {
        if (gameArea[y][x].isMine())
          continue;

        calculateNumberOfMinesAround(x, y);
      }
    }
}

void Area::calculateNumberOfMinesAround(int x, int y)
{ // рахуємо клількість мін навколо
    int counter = 0;

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
           if (j == x && i == y)
               continue;

           counter += at(j, i).isMine();
        }
    }
    gameArea[y][x].setValue(counter);
}

Cell& Area::at(int x, int y) { // доступ до самої клітини
  if (x < 0 || x >= width) {
      return stub; // даємо клітину "спеціальну" клітину
  }
  if (y < 0 || y >= height) {
      return stub;
  }

  return gameArea[y][x];
}




int Area::getHeight() const
{
    return height;
}

int Area::getWidth() const
{
    return width;
}

int Area::getNumberMines() const
{
    return numberOfMines;
}
