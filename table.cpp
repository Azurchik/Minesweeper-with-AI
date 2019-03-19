#include "table.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QStyledItemDelegate>

#include <QSizePolicy>
#include <QDebug>

Table::Table(QWidget *parent)
    : QTableWidget (parent)
    , sizeCell     (21) //ініціалізація розміру клітин
    , rightButtonF (false)
    , leftButtonF  (false)
{

    horizontalScrollBar()->hide();
    verticalScrollBar  ()->hide();

    horizontalHeader()->hide();
    verticalHeader  ()->hide();

    setWordWrap(false);

    setCornerButtonEnabled(false);
    setShowGrid(false);
    setTabKeyNavigation(false);

    setIconSize({sizeCell-3, sizeCell-3});
    setTextElideMode(Qt::TextElideMode::ElideNone);


    horizontalHeader()->setDefaultSectionSize(sizeCell);
    verticalHeader()->setDefaultSectionSize(sizeCell);


    setDefaultDropAction(Qt::DropAction::IgnoreAction);
    setDragDropMode(DragDropMode::NoDragDrop);
    setEditTriggers(EditTrigger::NoEditTriggers);
    setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
}


void Table::initializeSize()
{ // підбираєм розмір таблиці до його змісту
    int sizeRow    = (rowCount() * sizeCell) + 4;
    int sizeColumn = (columnCount() * sizeCell) + 4;

    setBaseSize   (sizeColumn, sizeRow);
    setMaximumSize(sizeColumn, sizeRow);
    setMinimumSize(sizeColumn, sizeRow);
}

void Table::mousePressEvent(QMouseEvent *event)
{ // подія "клавіша миші зажата"
    if (event->button() == Qt::LeftButton) {
        leftButtonF = true; // прапорець зажатої лівої клавіши миші
    }
    if (event->button() == Qt::RightButton) {
        rightButtonF = true; // прапорець зажатої правої клавіши миші
    }
}

void Table::mouseReleaseEvent(QMouseEvent *event)
{ // подія "клавіша миші віджата"
    QTableWidgetItem *item = itemAt(mapFromGlobal(QCursor::pos())); // записуєм адресу клітини під віджатої миші
    if (item == 0) // якщо немає клітини
        return;

    int col = item->column();
    int row = item->row();

    if (rightButtonF && leftButtonF) { // якщо були зажаті обидві клавіши
        emit l_r_Clicked(row, col); // посилаємо сигнал "подвійного нажимання" з координатами клітини

        updateFlags(event); // очищаємо прапорець тої клавіші, котра була зажата перша
        return;
    }

    if (event->button() == Qt::RightButton) { // віджата ліва клавіша
        emit rightClicked(row, col); // сигнал віджатої лівої клавіши з координатами клітини
    } else if (event->button() == Qt::LeftButton) { // віджата права клавіша
        emit leftClicked(row, col); // сигнал віджатої правої клавіши з координатами клітини
    }

    updateFlags(event); // очищаємо прапорець зажатої клавіші
}

void Table::updateFlags(QMouseEvent *event)
{ // очистка прапорів зажатих клавіш
    if (event->button() == Qt::RightButton) {
        rightButtonF = !rightButtonF;
    } else if (event->button() == Qt::LeftButton) {
        leftButtonF  = !leftButtonF;
    }
}
