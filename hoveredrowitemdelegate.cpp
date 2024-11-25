#include "hoveredrowitemdelegate.h"
#include <QPainter>

HoveredRowItemDelegate::HoveredRowItemDelegate(QTableWidget *parent)
    : QStyledItemDelegate{parent}
{
    tableWidget = parent;
}

void HoveredRowItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //选中状态或是在鼠标下方的状态
    if(option.state.testFlag(QStyle::State_Selected) || option.state.testFlag(QStyle::State_MouseOver))
    {
        QTableWidgetItem *hoveredItem = tableWidget->item(index.row(), index.column());
        if(hoveredItem)
        {
            const int row = hoveredItem->row();
            //遍历列，绘制此行的所有列
            for(int column = 0; column < tableWidget->columnCount(); column++)
            {
                QTableWidgetItem *item = tableWidget->item(row, column);
                if(item)
                {
                    QModelIndex index = tableWidget->model()->index(row, column);
                    QStyleOptionViewItem itemOption = option;
                    itemOption.index = index;
                    itemOption.rect = tableWidget->visualItemRect(item);
                    QStyledItemDelegate::paint(painter, itemOption, index);
                }
            }
        }
    }
    else
    {
        return QStyledItemDelegate::paint(painter, option, index);
    }
}
