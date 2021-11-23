#ifndef WAVELENGTHTABLE_H
#define WAVELENGTHTABLE_H

#include <QTableWidget>
#include "optical.h"
using namespace geopter;

/** Table widget for wavelength spec */
class WavelengthTable : public QTableWidget
{
    Q_OBJECT

public:
    WavelengthTable(QWidget* parent = nullptr);

    enum Column{
        Value,
        Weight,
        Color
    };

    void importWavelengthData(const std::shared_ptr<OpticalSystem> optsys);
    void applyCurrentData(std::shared_ptr<OpticalSystem> optsys);

private slots:
    /** Show context menu. This function is called when right click on the vertical header */
    void showContextMenu();

    /** Insert a new field to current row */
    void insertWavelength();

    /** Remove current field from the table */
    void removeWavelength();

    void addWavelength();

    /** Slot function for cell double click */
    void onDoubleClick(QTableWidgetItem* item);

private:
    void setWavelengthData(int row, const Wvl* wvl);
    void setupVerticalHeader();
    void postProcess();

    QColor rgbToQColor(const Rgb& rgb);
    Rgb QColorToRgb(const QColor& color);

    int m_displayDigit;
};

#endif // WAVELENGTHTABLE_H
