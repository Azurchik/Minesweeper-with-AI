#include "advert.h"
#include "ui_advert.h"

#include <QSettings>
#include <QDate>

Advert::Advert(bool gameWon, int time, QWidget *parent)
    : QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
              | Qt::WindowType::WindowCloseButtonHint)
    , restart(false)
    , ui(new Ui::Advert)
{
    ui->setupUi(this);

    QSettings settings("Azurchik", "Minesweeper"); // вказуєм чиї налаштування брати

    bool isCustom = false;
    QString difficulty; // з'ясовуєм, який рівень використовуєм
    if (settings.value("classic/options/bgn", true).toBool()) {
        difficulty = "Beginner";
    } else if (settings.value("classic/options/intm", false).toBool()) {
        difficulty = "Intermediate";
    } else if (settings.value("classic/options/advc", false).toBool()){
        difficulty = "Advanced";
    } else {
        isCustom = true;
    }

    ui->insertDate->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    ui->insertTime->setNum(time);

    if (!isCustom) {
        settings.beginGroup(difficulty);
        QString text;

        text.setNum(settings.value("bestTime", 999).toInt());
        text.append("с.");
        ui->insertBestTime->setText(text);

        float played = settings.value("numberGames", 0).toFloat();
        text.setNum(played);
        ui->insertPlayed->setText(text);

        int gamesWon = settings.value("gamesWon", 0).toInt();
        text.setNum(gamesWon);
        ui->insertWon->setText(text);


        text.setNum(gamesWon / (played / 100 ));
        text.append("%");
        ui->insertPercentage->setText(text);
        settings.endGroup();
    } else { // при підібраному рівні дані не записуются
        ui->labelBestTime->hide();
        ui->labelDate->hide();
        ui->labelPercentage->hide();
        ui->labelPlayed->hide();
        ui->labelWon->hide();

        ui->insertBestTime->hide();
        ui->insertDate->hide();
        ui->insertPercentage->hide();
        ui->insertPlayed->hide();
        ui->insertWon->hide();
    }

    if (gameWon) {
        setWindowTitle("Гра виграна");
        ui->pushRestart->hide();
    } else {
        setWindowTitle("Гра програна");
        ui->labelText->setText("Ви програли. Спробуйте ще раз!");
    }

}

Advert::~Advert()
{
    delete ui;
}

bool Advert::checkRestart()
{
    return restart;
}

void Advert::on_pushRestart_clicked()
{
    restart = true;
    close();
}
