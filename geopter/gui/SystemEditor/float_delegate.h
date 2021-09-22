#ifndef FLOATDELEGATE_H
#define FLOATDELEGATE_H

#include <QItemDelegate>
#include <QPainter>
#include <QString>

/**
 * @brief delegate and override the paint method
 * @example ui->tableWidget->setItemDelegate(FloatDelegate(3))
 *
 * https://stackoverflow.com/questions/45599857/qtablewidget-display-certain-decimals-like-excel
 */
class FloatDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    FloatDelegate(int decimals, QWidget* parent=nullptr) : QItemDelegate(parent){
        nDecimals_ = decimals;
    }

    ~FloatDelegate(){

    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override{
        auto var = index.model()->data(index,Qt::EditRole);
        try {
            bool isDouble;
            double num = var.toDouble(&isDouble);

            if(isDouble){
                drawBackground(painter, option,index);
                painter->drawText(option.rect, Qt::AlignRight, QString::number(num, 'f', nDecimals_));
            }else{
                QItemDelegate::paint(painter, option, index);
            }
        }  catch (...) {
            QItemDelegate::paint(painter, option, index);
        }
    }


private:
    int nDecimals_;

};

#endif // FLOATDELEGATE_H
