#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>

namespace Ui {
class Statistics;
}

class Statistics : public QDialog
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics();


private slots:
    void on_boxDifficulty_activated(const QString &arg1);

    void on_buttonReset_clicked();

    void on_buttonClose_clicked();

private:
    QString difficulty;

    Ui::Statistics *ui;
};

#endif // STATISTICS_H
