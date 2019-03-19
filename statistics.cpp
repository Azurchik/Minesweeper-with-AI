#include "statistics.h"
#include "ui_statistics.h"

#include <QSettings>
#include <QMessageBox>
#include <QPushButton>

Statistics::Statistics(QWidget *parent) :
    QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
            | Qt::WindowType::WindowCloseButtonHint),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    on_boxDifficulty_activated(ui->boxDifficulty->currentText()); // запуск події
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::on_boxDifficulty_activated(const QString &arg1)
{ // подія статистики вибраного рівня складності
    QSettings settings("Azurchik", "Minesweeper");
    // назви складності
    if (arg1 == "Початківець") {
        difficulty = "Beginner";
    } else if (arg1 == "Середній") {
        difficulty = "Intermediate";
    } else {
        difficulty = "Advanced";
    }

    settings.beginGroup(difficulty);
    QString text;

    text.setNum(settings.value("bestTime", -1).toInt());
    ui->insertDate->setText(settings.value("date").toString());
    text.append("с. ");
    ui->insertBestTime->setText(text);

    float played = settings.value("numberGames", 0).toFloat();
    text.setNum(played);
    ui->insertPlayed->setText(text);

    int gamesWon = settings.value("gamesWon", 0).toInt();
    text.setNum(gamesWon);
    ui->insertWon->setText(text);

    if (gamesWon != 0) {
        text.setNum(gamesWon / (played / 100 ));
    } else {
        text.setNum(0);
    }
    text.append("%");
    ui->insertPercentage->setText(text);


    text.setNum(settings.value("winStreak", 0).toInt());
    ui->insertWinStreak->setText(text);

    text.setNum(settings.value("loseStreak", 0).toInt());
    ui->insertLoseStreak->setText(text);

    text.setNum(settings.value("streak", 0).toInt());
    ui->insertCurStreak->setText(text);
    settings.endGroup();
}

void Statistics::on_buttonReset_clicked()
{ // очистити статистику даного рівня складності
    QMessageBox msgBox; // вікно з попередженням
    QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);
    QPushButton *resetButton = msgBox.addButton(tr("Скинути"), QMessageBox::AcceptRole);
    abortButton->setText("Скасувати");
    msgBox.setDefaultButton(abortButton);

    QString text = ui->boxDifficulty->currentText();
    text.prepend("Ви точно хочете видалити статистику рівня: ");
    msgBox.setWindowTitle("Скидання");
    msgBox.setText(text);
    msgBox.exec();

    if (msgBox.clickedButton() == resetButton) {
        QSettings settings("Azurchik", "Minesweeper");
        settings.beginGroup(difficulty);
        settings.remove("");
        settings.endGroup();

        settings.setValue("classic/options/reset", true);

        on_boxDifficulty_activated(ui->boxDifficulty->currentText());
    }
}

void Statistics::on_buttonClose_clicked()
{
    close();
}
