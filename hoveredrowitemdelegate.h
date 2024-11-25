#ifndef HOVEREDROWITEMDELEGATE_H
#define HOVEREDROWITEMDELEGATE_H


#include <QStyledItemDelegate>
#include <QTableWidget>

class HoveredRowItemDelegate : public QStyledItemDelegate
{
public:
    explicit HoveredRowItemDelegate(QTableWidget *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QTableWidget *tableWidget{};
};


#endif // HOVEREDROWITEMDELEGATE_H
