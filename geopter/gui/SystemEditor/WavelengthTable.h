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

signals:
    void setupCompleted();

    /** This signal is emitted when wavelength value is edited */
    void valueEdited();

public slots:

    /** Insert a new wavelength to current row */
    void insertWavelength();

    /** Remove current wavelength from the table */
    void removeWavelength();

    /** Add new wavelength row */
    void addWavelength();

    /** Slot function called when a cell is double clicked */
    void onDoubleClick(QTableWidgetItem* item);

    /** Slot function called when a cell is edited  */
    void onItemChanged(QTableWidgetItem* item);

private:
    void setWavelengthData(int row, const Wvl* wvl);
    void setupVerticalHeader();
    void setupItems();

    QColor rgbToQColor(const Rgb& rgb);
    Rgb QColorToRgb(const QColor& color);

    bool m_settingUp;
    int m_displayDigit;
};

#endif // WAVELENGTHTABLE_H
