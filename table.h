#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QMouseEvent>

// Таблиця для показу клітин
class Table : public QTableWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr);

    void initializeSize();

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void rightClicked(int row, int column);
    void leftClicked(int row, int column);
    void l_r_Clicked(int row, int column);

private:
    const int sizeCell;

    void updateFlags(QMouseEvent *event);
    bool rightButtonF;
    bool leftButtonF;
};

#endif // TABLE_H
