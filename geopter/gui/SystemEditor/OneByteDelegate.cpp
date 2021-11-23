#include "OneByteDelegate.h"
#include <QRegExp>
#include <iostream>

OneByteDelegate::OneByteDelegate(QWidget* parent)
    : QItemDelegate(parent)
{

}

void OneByteDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QVariant value = editor->property("text");

    QRegExp regOneByte("^[a-zA-Z0-9!-/:-@¥[-`{-~]*$");

    if( ! regOneByte.exactMatch(value.toString())){
        std::cerr << "Invalid input" << std::endl;
        value = QVariant(""); // empty
    }

    model->setData(index, value);
}

