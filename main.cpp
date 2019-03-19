#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{   // параметри для збереження налаштувань
    QCoreApplication::setOrganizationName("Azurchik");
    QCoreApplication::setApplicationName("Minesweeper");

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
