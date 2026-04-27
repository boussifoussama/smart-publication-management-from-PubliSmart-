#ifndef STOCKCOLORDELEGATE_H
#define STOCKCOLORDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QColor>

class StockColorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StockColorDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // Column 4 = quantite
        int qte = index.sibling(index.row(), 4).data().toInt();

        if (qte == 0)
            opt.palette.setColor(QPalette::Base, QColor(255, 160, 160)); // red
        else if (qte <= 5)
            opt.palette.setColor(QPalette::Base, QColor(255, 220, 100)); // orange

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

#endif // STOCKCOLORDELEGATE_H
