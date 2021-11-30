#ifndef LENSSPREADSHEET_H
#define LENSSPREADSHEET_H

#include <QTableWidget>
#include "SpreadSheetItem.h"
#include "optical.h"
using namespace geopter;

/** Lens data spread sheet
 *
 *  This table manages all surface data such as radius, thickness, material, etc.
 *
*/
class LensSpreadSheet : public QTableWidget
{
    Q_OBJECT

public:
    LensSpreadSheet(std::shared_ptr<OpticalSystem> optsys, QWidget *parent = nullptr);

    enum Column{
        Label, // editable
        SurfaceType,
        Radius, // editable
        Thickness, // editable
        Material, // editable
        Mode,
        SemiDiameter,
        Aperture,
    };

    /** set up UI with current optical system */
    void reload();

public slots:
    /** show context menu when double click on vertical header */
    void showContextMenuOnHeader();

    /** show context menu when double click on a cell */
    void showContextMenuOnCell();

    /** show surface property dialog */
    void showSurfacePropertyDlg();

    /** insert to current row */
    void insertSurface();

    /** remove current row */
    void removeSurface();

    /** Set current surface as stop */
    void setStop();

    /** Set display digit for float values */
    //void setDisplayDigit(int d);

    void updateUiSurfaceData(int si);

    void applyChange(QTableWidgetItem* item);


private:
    void setupVerticalHeader();
    void postProcess();

    void setValue(int row, int col, QVariant value, int role= Qt::EditRole);

    std::shared_ptr<OpticalSystem> m_opticalSystem;
    int m_displayDigit;
    bool m_isUserEdit;
};

#endif // LENSSPREADSHEET_H
