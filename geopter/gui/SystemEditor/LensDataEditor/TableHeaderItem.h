#ifndef TABLEHEADERITEM_H
#define TABLEHEADERITEM_H

//https://stackoverflow.com/questions/46469720/how-to-make-a-qheaderview-multilevel
//https://github.com/eyllanesc/stackoverflow/tree/master/questions/46469720

#include <QHash>
#include <QPair>
#include <QVariant>

class TableHeaderItem {
public:
    TableHeaderItem(TableHeaderItem *parent = 0);
    TableHeaderItem(int row, int column, TableHeaderItem *parent = 0);

    TableHeaderItem *insertChild(int row, int col);
    TableHeaderItem *child(int row, int col);

    TableHeaderItem *parent();

    int row() const;
    int column() const;

    void setData(const QVariant &data, int role);
    QVariant data(int role) const;

    void clear();

private:
    TableHeaderItem *m_parentItem;
    int _row;
    int _column;

    QHash<QPair<int, int>, TableHeaderItem *> m_childItems;
    QHash<int, QVariant> m_itemData;
};

#endif // TABLEHEADERITEM_H
