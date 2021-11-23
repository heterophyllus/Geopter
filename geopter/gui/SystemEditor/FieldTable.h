#ifndef FIELDTABLE_H
#define FIELDTABLE_H

#include <QTableWidget>
#include "optical.h"
using namespace geopter;

/** Table widget for fields specification */
class FieldTable : public QTableWidget
{
    Q_OBJECT

public:
    FieldTable(QWidget* parent = nullptr);

    enum Column{
        X,
        Y,
        Weight,
        Color,
        VUY,
        VLY,
        VUX,
        VLX
    };

    void importFieldData(const std::shared_ptr<OpticalSystem> optsys);
    void applyCurrentData(std::shared_ptr<OpticalSystem> optsys);

public slots:
    /** Show context menu. This function is called when right click on the vertical header */
    void showContextMenuOnHeader(const QPoint &pos);

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
    void postProcess();

    QColor rgbToQColor(const Rgb& rgb);
    Rgb QColorToRgb(const QColor& color);

    int m_displayDigit;
};

#endif // FIELDTABLE_H
