#ifndef FIELD_TABLE_VIEW_H
#define FIELD_TABLE_VIEW_H

#include <QTableView>
#include "FieldTableModel.h"
#include "optical.h"
using namespace geopter;

class FieldTableView : public QTableView
{
    Q_OBJECT

public:
    explicit FieldTableView(QWidget* parent = nullptr);

    void init();

public slots:
    void onDoubleClick(const QModelIndex &index);
    void insertRow();
    void removeRow();
    void addRow();

private:
    FieldTableModel* m_model;
    int m_digit;
};

#endif //FIELD_TABLE_VIEW_H
