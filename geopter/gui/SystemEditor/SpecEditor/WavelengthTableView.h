#ifndef WAVELENGTH_TABLE_VIEW_H
#define WAVELENGTH_TABLE_VIEW_H

#include <QTableView>
#include "WavelengthTableModel.h"

class WavelengthTableView : public QTableView
{
    Q_OBJECT

public:
    explicit WavelengthTableView(QWidget* parent = nullptr);

signals:
    void valueEdited();

private slots:
    void onDoubleClick(const QModelIndex &index);
    void insertRow();
    void removeRow();
    void addRow();
    void emitValueEdited();

private:
    WavelengthTableModel* m_model;
    int m_digit;
};


#endif //WAVELENGTH_TABLE_VIEW_H
