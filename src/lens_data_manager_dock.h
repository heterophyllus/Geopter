#ifndef LENS_DATA_MANAGER_DOCK_H
#define LENS_DATA_MANAGER_DOCK_H


#include <QTableWidget>

#include "spec_setting_dialog.h"

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "optical_core.h"
using namespace geopter;


/** Column labels for lens data spreadsheet */
enum Column{
    Label,
    SurfaceType,
    Radius,
    Thickness,
    Material,
    Mode,
    SemiDiameter,
    ApertureShape,
    MaxAperture,
};


/** Lens data spreadsheet.
 *  This dock is never closed.
 */
class LensDataManagerDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit LensDataManagerDock(std::shared_ptr<OpticalModel> opt_model, QString label, QWidget *parent = nullptr);
    ~LensDataManagerDock();

    void initialize();
    void setOpticalModel(std::shared_ptr<OpticalModel> opm);

    /** Set up table using optical model */
    void syncTableWithModel();

private slots:
    void showSpecSettingDlg();

    /** show context menu when cursor is on the vertical header */
    void showContextMenuOnHeader();

    /** show context menu when cursor is on the cells */
    void showContextMenuOnCell();

    /** insert a row (dummy surface) before the current*/
    void insertBefore();

    /** insert a row (dummy surface) after the current*/
    void insertAfter();

    /** remove current row */
    void remove();

    void showSurfacePropertyDlg(int si = -1);

    /** Validate cell input by user */
    void validateCellInput(int row, int col);

    void validateItemDoubleClicked(QTableWidgetItem* item);
    void validateCellDoubleClicked(int row, int col);

private:
    QTableWidget* table_;
    QToolBar* toolbar_;
    std::shared_ptr<OpticalModel> opt_model_;
    int stop_surface_;

    std::unique_ptr<SpecSettingDialog> specSettingDlg_;

    void updateVerticalHeader();
    void insertRow(int row);
    void setValueToCell(int row, int col, double val);
    void setValueToCell(int row, int col, QString str);

    /** Fill in cells in a row of the table using optical model */
    void syncRowWithModel(int row);

    bool validateLabelInput(int row);
    bool validateRadiusInput(int row);
    bool validateThicknessInput(int row);
    bool validateMediumInput(int row);

    void setCellEditable(int row, int col, bool state);

    void updateRowColor(int row);
    void setColorToCell(int row, int col, QColor color);

    void connectValidateCell(bool isConnected);

};

#endif // LENS_DATA_MANAGER_DOCK_H
