#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H

#include <QItemDelegate>

class FloatDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    FloatDelegate(int decimals, bool allowInf= true, QWidget* parent=nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setDecimal(int decimal){
        nDecimals_ = decimal;
    }

protected:
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;


private:
    int nDecimals_;
    bool m_allowInf;
};

#endif // CELLDELEGATE_H
