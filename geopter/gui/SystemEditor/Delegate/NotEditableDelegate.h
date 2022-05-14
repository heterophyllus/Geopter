#ifndef NO_EDITABLE_DELEGATE_H
#define NO_EDITABLE_DELEGATE_H

#include <QItemDelegate>

//https://stackoverflow.com/questions/8795510/how-to-edit-a-qtableview

class NotEditableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NotEditableDelegate(QObject *parent = 0)
        : QItemDelegate(parent)
    {}

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index){
        return false;
    }

    QWidget* createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const{
        return Q_NULLPTR;
    }

};

#endif
