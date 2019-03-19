#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QPixmap>
#include <QSize>
#include <QDate>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
                  | Qt::WindowType::WindowCloseButtonHint)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap time (":/classic/images/time.png");
    QPixmap mines(":/classic/images/mines.png");
    time.scaled(32, 32, Qt::IgnoreAspectRatio);
    mines.scaled(32, 32, Qt::IgnoreAspectRatio);
    ui->labelTime->setPixmap(time); // встановлюємо злбраження на форму
    ui->labelMines->setPixmap(mines);

    table    = new Table (this);
    timer    = new QTimer(this);
    settings = new QSettings("Azurchik", "Minesweeper"); // вказуєм чиї налаштування брати

    ui->gridLayout->addWidget(table);

    startGame();

    settings->beginGroup("preservationClassic"); // вікно виявлення збереження
    bool gameSaved = settings->value("save", false).toBool();
    settings->endGroup();
    if (gameSaved) {
        if (settings->value("classic/options/continue", false).toBool()) {
            loadGame();
        } else {
            QMessageBox msg(this);
            msg.setModal(true);
            msg.setText("Ви бажаєте запустити збережену гру?");
            msg.setWindowTitle("Знайденa збережена гра");
            msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);

            QCheckBox *chContinue = new QCheckBox();
            chContinue->setText("Завжди запускати збережені ігри");
            msg.setCheckBox(chContinue);

            msg.setButtonText(QMessageBox::Yes, "Так");
            msg.setButtonText(QMessageBox::No, "Ні");
            msg.setDefaultButton(QMessageBox::Yes);
            int ret = msg.exec();

            if (chContinue->isChecked())
                settings->setValue("classic/options/continue", true);
            if (ret == QMessageBox::Yes)
                loadGame();
        }

    }

    connect(table, SIGNAL(rightClicked(int, int)),
            this, SLOT(rightClicked(int, int)));

    connect(table, SIGNAL(leftClicked(int, int)),
            this, SLOT(leftClicked(int, int)));

    connect(table, SIGNAL(l_r_Clicked(int, int)),
            this, SLOT(lAndRClicked(int, int)));

    connect(timer, SIGNAL(timeout()),
            this, SLOT(incrementLcd()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateSize()
{
    int w = table->maximumWidth() + 22;
    int h = ui->lcdLayout->sizeHint().height()
            + table->maximumHeight()
            + ui->menuBar->size().height()
            + 25;

    QSize size(w, h);

    resize(size);
    setFixedSize(size);
}




void MainWindow::updateTable()
{
    for (int y = 0; y < area->getHeight(); y++) {
        for (int x = 0; x < area->getWidth(); x++) {
            int val = controller->getValue(x, y);

            QString path;
            path.setNum(val);
            path.prepend(":/classic/images/");
            path.append(".png");

            QPixmap pix(path);
            pix.scaled(21, 21, Qt::IgnoreAspectRatio);

            table->item(y, x)->setIcon({pix});
        }

    }

    table->update();
}

void MainWindow::updateStatistics(bool gameWon)
{
    QString difficulty; // з'ясовуєм, який рівень використовуєм
    if (settings->value("classic/options/bgn", true).toBool()) {
        difficulty = "Beginner";
    } else if (settings->value("classic/options/intm", false).toBool()) {
        difficulty = "Intermediate";
    } else {
        difficulty = "Advanced";
    }

    settings->beginGroup(difficulty);
    int numberGames = settings->value("numberGames", 0).toInt();
    settings->setValue("numberGames", ++numberGames);

    int streak = settings->value("streak", 0).toInt();

    if (gameWon) {
        int gamesWon = settings->value("gamesWon", 0).toInt();
        settings->setValue("gamesWon", ++gamesWon);

        if (streak <= 0) {
           settings->setValue("streak", 1);
        } else {
            settings->setValue("streak", ++streak);

            int winStreak = settings->value("winStreak", 0).toInt();
            if (winStreak < streak)
                settings->setValue("winStreak", streak);
        }

        int bestTime = settings->value("bestTime", 999).toInt();
        int currTime = ui->lcdTime->intValue();
        if (currTime < bestTime) {
            settings->setValue("bestTime", currTime);
            settings->setValue("date", QDate::currentDate().toString("dd.MM.yyyy"));
        }

    } else {
        if (streak >= 0) {
           settings->setValue("streak", -1);
        } else {
            settings->setValue("streak", --streak);

            int loseStreak = settings->value("loseStreak", 0).toInt();
            if (loseStreak > streak)
                settings->setValue("loseStreak", streak);
        }

    }

    settings->endGroup();
}

void MainWindow::startGame()
{
    timer->stop();
    table->clearContents();

    int w = settings->value("classic/field/width", 9).toInt();
    int h = settings->value("classic/field/height", 9).toInt();
    int m = settings->value("classic/field/mines", 10).toInt();

    area       = new Area(w, h, m); // поле, на якому самі міни
    controller = new Controller(area); // контроллер, який показує їх

    f_Bot     = false; // флажок роботи бота
    f_FStep   = true; // флажок першого ходу
    f_Restart = false; // флажок перезапуску

    ui->lcdTime->setVisible(!settings->value("classic/options/time", true).toBool());
    ui->lcdMines->display(controller->minesLeft());
    ui->lcdTime->display(0);

    table->setRowCount(h);
    table->setColumnCount(w);
    table->initializeSize();

    QPixmap pix(":/classic/images/9.png");
    pix.scaled(15, 15, Qt::IgnoreAspectRatio);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            table->setItem(y, x, new QTableWidgetItem(pix, nullptr));
        }
    }

    table->update();
    updateSize();
}

void MainWindow::restartGame()
{
    controller->reload();

    ui->lcdMines->display(controller->minesLeft());
    ui->lcdTime->display(0);

    for (int y = 0; y < area->getHeight(); ++y) {
        for (int x = 0; x < area->getWidth(); ++x) {
            Cell &cell = area->at(x, y);

            if (cell.isFlagged()) {
                cell.toggleFlag();
            } else if (cell.isOpened()) {
                cell.close();
            }
        }
    }

    f_Restart = true;
    updateTable();
}

void MainWindow::saveGame()
{
    timer->stop();
    QString resultState;
    QString resultArea;
    int prevNumberState = 0; // попереднє число стану
    int prevNumberArea  = 0; // попереднє значеня клітини
    int countState = 1;
    int countArea  = 1;

    bool firstNumber = true;
    for (int y = 0; y < area->getHeight(); ++y) {
        for (int x = 0; x < area->getWidth(); ++x) {
            int number = controller->getValue(x, y); // для збереження стану клітини
            int cell   = area->at(x, y).getValue(); // для збереження значення клітини

            if (firstNumber) {
                prevNumberState = number;
                prevNumberArea  = cell;
                firstNumber = false;
                continue;
            }
            // 9\9\9\9 => 4.9 // 9\0\0\9 => 9 2.0 9
            if (prevNumberState == number) {
                ++countState;
            } else {
                if (countState != 1) {
                    resultState += QString::number(countState) + '.';
                    countState = 1;
                }
                resultState += QString::number(prevNumberState);
                resultState += ' ';

                prevNumberState = number;
            }

            if (prevNumberArea == cell) {
                ++countArea;
            } else {
                if (countArea != 1) {
                    resultArea += QString::number(countArea) + '.';
                    countArea = 1;
                }
                resultArea += QString::number(prevNumberArea);
                resultArea += ' ';

                prevNumberArea = cell;
            }
        }
    }
    // якщо останнє число було такеж саме як і попереднє
    if (countState != 1)
        resultState += QString::number(countState) + '.' + QString::number(prevNumberState);
    if (countArea  != 1)
        resultArea  += QString::number(countArea ) + '.' + QString::number(prevNumberArea);

    settings->beginGroup("preservationClassic");
    settings->setValue("state", resultState);
    settings->setValue("area" , resultArea);
    settings->setValue("time", ui->lcdTime->intValue());
    settings->setValue("save", true);
    settings->endGroup();
}

void MainWindow::loadGame()
{
    settings->beginGroup("preservationClassic");
    QString strState = settings->value("state").toString(); // список чисел стану
    QString strArea  = settings->value("area").toString(); //  список значенень клітин
    ui->lcdTime->display(settings->value("time").toInt());
    settings->setValue("save", false);
    settings->endGroup();

    QStringList listState = strState.split(" ", QString::SkipEmptyParts);
    QStringList listArea  = strArea.split (" ", QString::SkipEmptyParts);

    std::vector<int> field;
    std::vector<int> area_;

    for (int i = 0; i < listState.size(); ++i) {
        strState = listState[i];

        if (strState.contains(".")) { // 4.9 => 9\9\9\9
            QStringList cells = strState.split(".");
            int quantity = cells[0].toInt();
            int cell     = cells[1].toInt();

            for (int j = 0; j < quantity; ++j)
                field.push_back(cell);

        } else { // 9 => 9
            field.push_back(strState.toInt());
        }
    }

    for (int i = 0; i < listArea.size(); ++i) {
        strArea  = listArea [i];

        if (strArea.contains(".")) {
            QStringList cells = strArea.split(".");
            int quantity = cells[0].toInt();
            int cell     = cells[1].toInt();

            for (int j = 0; j < quantity; ++j)
                area_.push_back(cell);

        } else {
            area_.push_back(strArea.toInt());
        }
    }

    int counter = 0;
    for (int y = 0; y < area->getHeight(); ++y) {
        for (int x = 0; x < area->getWidth(); ++x) {
            Cell &cell = area->at(x, y);
            int number = field[counter];
            int value  = area_[counter];

            cell.setValue(value);

            if (number < 9) { // всі числа менше 9 - відкриті
                cell.open();
            } else if (number == 9) {
                cell.close();
            } else if (number == 10){
                cell.toggleFlag();
            }

            counter++;
        }
    }

    f_FStep = false;
    timer->start(1000);
    updateTable();
}




void MainWindow::closeEvent(QCloseEvent *event) // перевизначення методу закриття вікна
{
    if (f_FStep || f_Bot || controller->gameWon()
            || controller->gameLost() || f_Restart) {
        event->accept();
        return;
    }

    if (!settings->value("classic/options/save", false).toBool()) {
        QMessageBox msgBox(this);
        msgBox.setModal(true);

        QPushButton *saveButton   = msgBox.addButton(tr("Зберегти"), QMessageBox::AcceptRole);
        QPushButton *abortButton  = msgBox.addButton(QMessageBox::Abort);
        QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);

        abortButton->setText("Вийти");
        cancelButton->setText("Скасувати");
        msgBox.setDefaultButton(saveButton);

        msgBox.setWindowTitle("Вихід з ігри");
        msgBox.setText("Ви бажаєте зберегти прогрес цієї гри?");
        msgBox.exec();

        if (msgBox.clickedButton() == saveButton) {
            saveGame();
            event->accept();
            return;
        } else if (msgBox.clickedButton() == abortButton) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        saveGame();
        event->accept();
    }


    if (!(controller->gameWon() || controller->gameLost())) { // якщо вихід посеред гри
        timer->stop();
        updateStatistics(false);
    }
}

void MainWindow::incrementLcd() // лічильник часу
{
    int num = ui->lcdTime->intValue();
    ui->lcdTime->display(++num);

    if (num == 999)
        timer->stop();
}



void MainWindow::leftClicked(int row, int column)
{ // подія для сигналу при натисканні лівої кнопки миші
    if (f_Bot) // якщо бот запущенний, то кнопка не дійсна
        return;

    if (f_Restart) { // при перезапуску ініціалізація не потрібна
        timer->start(1000);
        f_Restart = false;
    }

    if (f_FStep) {
        controller->firstStep(column, row); // ініціалізуємо поле
        timer->start(1000);
        f_FStep = false;
    } else {
        controller->openCell(column, row);
    }
    // якщо відкрита була остання клітина, або попадання на міну
    if (controller->gameWon() || controller->gameLost()) {
        timer->stop();
        updateStatistics(controller->gameWon());
        endGame(controller->gameWon());
        advertWindow(controller->gameWon());
        return;
    }
    updateTable();
}

void MainWindow::rightClicked(int row, int column)
{ // подія для сигналу при натисканні правої кнопки миші
    if (f_Bot)
        return;

    controller->toggleFlag(column, row); // ставиться/знімається прапор
    ui->lcdMines->display(controller->minesLeft()); // поновлення дисплею мін на формі
    updateTable();

}

void MainWindow::lAndRClicked(int row, int column)
{ // подія для сигналу при натисканні правої і лівої кнопок миші
    if (f_Bot)
        return;

    controller->openCellsAround(column, row);

    if (controller->gameWon() || controller->gameLost()) { // якщо відкрита була остання клітина, або попадання на міну
        timer->stop();
        updateStatistics(controller->gameWon());
        endGame(controller->gameWon());
        advertWindow(controller->gameWon());
        return;
    }
    updateTable();
}

void MainWindow::endGame(bool gameWon)
{ // показ правильних, неправильних і невідкритих клітин на таблиці
    if (!gameWon) {
        QVector<BCell> listWrongMines;
        QVector<BCell> listUncheckedMines;
        QVector<BCell> listTriggeredMines;

        for (int y = 0; y < area->getHeight(); ++y) {
            for (int x = 0; x < area->getWidth(); ++x) {

                if (area->at(x, y).isFlagged()) {
                    if (!area->at(x, y).isMine())
                        listWrongMines.push_back({x, y});

                } else if (area->at(x, y).isMine()) {
                    if (area->at(x, y).isOpened())
                        listTriggeredMines.push_back({x, y});

                    if (!area->at(x, y).isFlagged())
                        listUncheckedMines.push_back({x, y});
                }

            }
        }

        QPixmap map(":/classic/images/wrongMines.png");
        map.scaled(21, 21, Qt::IgnoreAspectRatio);
        for (BCell &cell: listWrongMines)
            table->item(cell.getY(), cell.getX())->setIcon({map});

        QPixmap map1(":/classic/images/uncheckedMines.png");
        for (BCell &cell: listUncheckedMines)
            table->item(cell.getY(), cell.getX())->setIcon({map1});

        QPixmap map2(":/classic/images/triggeredMines.png");
        for (BCell &cell: listTriggeredMines)
            table->item(cell.getY(), cell.getX())->setIcon({map2});

        table->update();
    } else {
        updateTable();
    }    
}

void MainWindow::advertWindow(bool gameWon)
{ // вікно об'яви завершення гри
    Advert advert(gameWon, ui->lcdTime->intValue(), this);
    advert.setModal(true);
    int result = advert.exec();

    if (advert.checkRestart()) { // якщо гра програна, то можна перезапустити гру
        restartGame();
        return;
    }

    if (result == QDialog::Accepted) {
        startGame();
    } else {
        close();
    }
}



void MainWindow::on_actionStatistics_triggered()
{
    Statistics statistics(this);
    statistics.setModal(true);
    statistics.exec();
}

void MainWindow::on_actionNewGame_triggered()
{ // подія на натисканні клавіша "Нова гра" на панелі меню
    if (!f_FStep) { // якщо ініціалізація вже була, з'являється вікно попередження
        QMessageBox msg(this);
        msg.setModal(true);
        msg.setText("Що ви бажаєте зробити з прогресом гри?");
        msg.setWindowTitle("Нова гра");
        msg.setInformativeText("При завершені або перезапуски гри - зараховується поразка.");

        QPushButton *nGameButton   = msg.addButton(tr("Завершити і розпочати нову гру"), QMessageBox::AcceptRole);
        QPushButton *restartButton = msg.addButton(QMessageBox::Abort);
        QPushButton *cancelButton  = msg.addButton(QMessageBox::Cancel);

        restartButton->setText("Перезапустити цю гру");
        cancelButton->setText("Продовжити грати");
        msg.setDefaultButton(nGameButton);
        int ret = msg.exec();

        if (ret == QMessageBox::AcceptRole) {
            timer->stop();
            updateStatistics(false);
            endGame(false);
            startGame();
        } else if (ret == QMessageBox::Abort) {
            restartGame();
        }
    } else {
        startGame();
    }
}

void MainWindow::on_actionOptions_triggered()
{ // подія на натисканні клавіша "Настройки" на панелі меню
    Options opt(this);
    opt.setModal(true);

    if(opt.exec() == QDialog::Accepted) {
        startGame();
    }
}

void MainWindow::on_actionExit_triggered()
{ // подія на натисканні клавіша "Вихід" на панелі меню
    close();
}

void MainWindow::on_actionStartBot_triggered()
{ // подія на натисканні клавіша "Запуск бота" на панелі меню
    timer->stop();
    ui->lcdTime->display("---");

    if (controller->gameWon() || controller->gameLost()) {
        QMessageBox ms(this);
        ms.setModal(true);
        ms.setWindowTitle("Бот");
        ms.setText("Гра закінчена");
        ms.exec();
        return;
    }

    f_Bot = true; // встановлюємо прапорець роботи бота
    bot = new Bot(controller);

    if (f_FStep) {
        bot->firstStep();
        f_FStep = false;
        updateTable();
    }

    do {
        // програма не встигає швидко промальовувати таблицю, тому робимо паузу на 1 секунду
        QTime dieTime = QTime::currentTime().addSecs(1);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        if (!bot->easySearch()) {
            if (!bot->exception()) {
                if (!bot->lastSteps()) {
                    bot->probability();
                }
            }
        }

        ui->lcdMines->display(controller->minesLeft());
        bot->openCells();

        updateTable();
    } while(!(controller->gameWon() || controller->gameLost()));

    endGame(controller->gameWon());

    QMessageBox msg(this);
    QString text;
    if (controller->gameWon()) {
        text = "Бот виграв";
    } else {
        text = "Бот програв";
    }
    msg.setModal(true);
    msg.setText(text);
    msg.setWindowTitle("Гра закінчена");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();

    startGame();
}
