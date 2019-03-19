#ifndef ADVERT_H
#define ADVERT_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class Advert;
}

class Advert : public QDialog
{
    Q_OBJECT

public:
    explicit Advert(bool gameWon, int time, QWidget *parent = nullptr);
    ~Advert();

    bool checkRestart();

private slots:

    void on_pushRestart_clicked();

private:
    bool restart;

    Ui::Advert *ui;
};

#endif // ADVERT_H
