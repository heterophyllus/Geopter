#ifndef ONEBYTEDELEGATE_H
#define ONEBYTEDELEGATE_H

#include <QItemDelegate>

class OneByteDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    OneByteDelegate(QWidget* parent=nullptr);

protected:
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;

};

#endif // ONEBYTEDELEGATE_H
