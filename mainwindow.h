#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include "statistics.h"
#include "options.h"
#include "advert.h"

#include "area.h"
#include "controller.h"
#include "bot.h"
#include "table.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startGame();
    void restartGame();

    void saveGame();
    void loadGame();

private slots:
    void closeEvent(QCloseEvent *event);
    void incrementLcd();

    void leftClicked(int row, int column);
    void rightClicked(int row, int column);
    void lAndRClicked(int row, int column);

    void on_actionStatistics_triggered();
    void on_actionNewGame_triggered();
    void on_actionOptions_triggered();
    void on_actionExit_triggered();
    void on_actionStartBot_triggered();

private:
    void updateStatistics(bool gameWon);
    void endGame(bool gameWon);
    void advertWindow(bool gameWon);

    void advert();

    void updateSize();
    void updateTable();

    QSettings *settings;
    QTimer *timer;

    Table *table;

    bool f_Bot;
    bool f_FStep;
    bool f_Restart;

    Controller *controller;
    Bot *bot;
    Area *area;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
