#include "options.h"
#include "ui_options.h"

#include <QSettings>
#include <QRegExp>

#include <QDebug>

Options::Options(QWidget *parent) :
    QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
            | Qt::WindowType::WindowCloseButtonHint),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    QSettings settings("Azurchik", "Minesweeper"); // вказуєм чиї налаштування брати
    // з налаштувань берем стан елементів
    ui->radioBgn->setChecked   (settings.value("classic/options/bgn", true).toBool());
    ui->radioIntm->setChecked  (settings.value("classic/options/intm", false).toBool());
    ui->radioAdvc->setChecked  (settings.value("classic/options/advc", false).toBool());
    ui->radioCustom->setChecked(settings.value("classic/options/custom", false).toBool());

    ui->checkContinue->setChecked(settings.value("classic/options/continue", false).toBool());
    ui->checkSaved->setChecked   (settings.value("classic/options/save", false).toBool());
    ui->checkTime->setChecked    (settings.value("classic/options/time", false).toBool());

    if (ui->radioCustom->isChecked()) {
        ui->lineWidth->setText (settings.value("classic/field/width").toString());
        ui->lineHeight->setText(settings.value("classic/field/height").toString());
        ui->lineMines->setText (settings.value("classic/field/mines").toString());
    }
}

Options::~Options()
{
    delete ui;
}

void Options::on_radioCustom_toggled(bool checked)
{ // подія коли кнопка вибрана
    ui->labelHeight->setEnabled(checked);
    ui->labelWidth->setEnabled (checked);
    ui->labelMines->setEnabled (checked);

    ui->lineHeight->setEnabled(checked);
    ui->lineWidth->setEnabled (checked);
    ui->lineMines->setEnabled (checked);
}

void Options::on_pushButton_clicked()
{   //Збереження налаштувань
    QSettings settings("Azurchik", "Minesweeper");

    settings.setValue("classic/options/bgn", ui->radioBgn->isChecked());
    settings.setValue("classic/options/intm", ui->radioIntm->isChecked());
    settings.setValue("classic/options/advc", ui->radioAdvc->isChecked());
    settings.setValue("classic/options/custom", ui->radioCustom->isChecked());

    settings.setValue("classic/options/continue", ui->checkContinue->isChecked());
    settings.setValue("classic/options/save", ui->checkSaved->isChecked());
    settings.setValue("classic/options/time", ui->checkTime->isChecked());

    if (ui->radioBgn->isChecked()) {
        settings.setValue("classic/field/width", 9);
        settings.setValue("classic/field/height",9);
        settings.setValue("classic/field/mines", 10);
    }
    if (ui->radioIntm->isChecked()) {
        settings.setValue("classic/field/width", 16);
        settings.setValue("classic/field/height",16);
        settings.setValue("classic/field/mines", 40);
    }

    if (ui->radioAdvc->isChecked()) {
        settings.setValue("classic/field/width", 30);
        settings.setValue("classic/field/height",16);
        settings.setValue("classic/field/mines", 99);
    }

    if (ui->radioCustom->isChecked()) {
        settings.setValue("classic/field/width", ui->lineWidth->text().toInt());
        settings.setValue("classic/field/height",ui->lineHeight->text().toInt());
        settings.setValue("classic/field/mines", ui->lineMines->text().toInt());
    }
    accept();
}
