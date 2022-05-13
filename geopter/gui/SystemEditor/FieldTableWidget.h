#ifndef FIELDTABLE_H
#define FIELDTABLE_H

#include <QTableWidget>
#include "system/optical_system.h"
using namespace geopter;

/** Table widget for fields specification */
class FieldTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    FieldTableWidget(QWidget* parent = nullptr);

    void importFieldData(const std::shared_ptr<OpticalSystem> optsys);
    void applyCurrentData(std::shared_ptr<OpticalSystem> optsys);

public slots:

    /** Insert a new field to current row */
    void insertField();

    /** Remove current field from the table */
    void removeField();

    void addField();

    /** Slot function for cell double click */
    void onDoubleClick(QTableWidgetItem* item);

private:
    void setFieldData(int row, const Field* fld= nullptr);
    void setupVerticalHeader();
    void setupItems();

    QColor rgbToQColor(const Rgb& rgb);
    Rgb QColorToRgb(const QColor& color);

    int m_displayDigit;
};

#endif // FIELDTABLE_H
