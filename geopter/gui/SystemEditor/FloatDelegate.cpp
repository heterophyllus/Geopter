#include "FloatDelegate.h"
#include <iostream>
#include <QPainter>
#include <QLineEdit>
#include <QDebug>

FloatDelegate::FloatDelegate(int decimals, bool allowInf, QWidget* parent)
    : QItemDelegate(parent)
{
    nDecimals_ = decimals;
    m_allowInf = allowInf;
}

void FloatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto var = index.model()->data(index,Qt::EditRole);
    try {
        bool isDouble;
        double num = var.toDouble(&isDouble);

        if(isDouble){
            drawBackground(painter, option, index);

            if(qIsInf(num)){
                painter->drawText(option.rect, Qt::AlignRight, "Infinity");
            }else{
                painter->drawText(option.rect, Qt::AlignRight, QString::number(num, 'f', nDecimals_));
            }
        }else{
            QItemDelegate::paint(painter, option, index);
        }
    }  catch (...) {
        QItemDelegate::paint(painter, option, index);
    }
}


void FloatDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QVariant value = editor->property("text");


    bool isNumber = false;
    double number = value.toDouble(&isNumber);
    if(isNumber){
        if(!m_allowInf){
            if(qIsInf(number)){
                std::cerr << "Infinity is not allowed to this cell" << std::endl;
                value = QVariant(10000000);
            }
        }
    }else{ // not a number
        std::cerr << "Not a number: " << value.toString().toStdString();
    }


    model->setData(index, value);
}

