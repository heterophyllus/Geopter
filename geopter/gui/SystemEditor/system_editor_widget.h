#ifndef SYSTEM_EDITOR_WIDGET_H
#define SYSTEM_EDITOR_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QColor>

#include "optical.h"

namespace Ui {
class SystemEditorWidget;
}


/** Column labels for lens data spreadsheet */
enum AssemblyTableColumn{
    Label,
    SurfaceType,
    Radius,
    Thickness,
    Material,
    Mode,
    SemiDiameter
};

enum WavelengthTableColumn{
    WavelengthValue,
    WavelengthWeight,
    WavelengthColor
};

enum FieldTableColumn{
    FieldX,
    FieldY,
    FieldWeight,
    FieldColor,
    FieldVUY,
    FieldVLY,
    FieldVUX,
    FieldVLX
};

class SystemEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemEditorWidget(std::shared_ptr<geopter::OpticalSystem> opt_sys, QWidget *parent = nullptr);
    ~SystemEditorWidget();

    void setOpticalSystem(std::shared_ptr<geopter::OpticalSystem> opt_sys);

    /** Update UI using current optical system */
    void syncUiWithSystem();

    /** Update Opticalsystem using current input data */
    void syncSystemWithUi();

private slots:

    //*******************************************************************************
    //  Assembly Tab
    /** show context menu when cursor is on the vertical header */
    void showContextMenuOnAssemblyTableHeader();

    /** show context menu when cursor is on the cells */
    void showContextMenuOnAssemblyTableCell();

    /** insert a line (dummy surface) before the current*/
    void insertBeforeCurrentLineOnAssemblyTable();

    /** insert a line (dummy surface) after the current*/
    void insertAfterCurrentLineOnAssemblyTable();

    /** remove current line */
    void removeLineFromAssemblyTable();

    /** Show property dialog for the current surface */
    void showSurfacePropertyDlg(int si = -1);

    /** Validate cell input by user */
    void validateCellInputOnAssemblyTable(int row, int col);

    /** Validate double click action on the cell */
    void validateCellDoubleClickOnAssemblyTable(QTableWidgetItem *item);


    //*********************************************************************************
    // Spec Tab
    /** Show context menu. This function is called when right click on wavelength table*/
    void showContextMenuOnWavelengthTableHeader();

    /** Show context menu. This function is called when right click on field table*/
    void showContextMenuOnFieldTableHeader();

    void addWavelength();

    void removeWavelength();

    void addField();

    void removeField();

    void validateCellDoubleClickOnWavelengthTable(int row, int col);
    void validateCellDoubleClickOnFieldTable(int row, int col);

private:
    /** Set up all tables */
    void initialize();

    /** Insert a line on assembly table */
    void insertLineOnAssemblyTable(int row);

    /** Update vertical header labels of assembly table */
    void updateVerticalHeaderOnAssemblyTable();

    void setTableEditable(bool state);

    void setConnectionValidateCellInput(bool state);

    void validateLabelInput(int row);
    void validateRadiusInput(int row);
    void validateThicknessInput(int row);
    void validateMaterialInput(int row);

    void updateReferenceWavelengthCombo();


    /** Set content(value, string, color, etc) to the given cell */
    void setContentToCell(QTableWidget *table, int row, int col, double value);
    void setContentToCell(QTableWidget *table, int row, int col, QString str);
    void setContentToCell(QTableWidget *table, int row, int col, QColor color);

    /** Convert geopter::Rgb to QColor */
    QColor rgbToQColor(geopter::Rgb rgbColor);

    /** Convert QColor to geopter::Rgb */
    geopter::Rgb QColorToRgb(QColor color);

    Ui::SystemEditorWidget *ui;

    std::shared_ptr<geopter::OpticalSystem> opt_sys_;

    const int assemblyTableColumnCount = 7;
    const int wavelengthTableColumnCount = 3;
    const int fieldTableColumncount = 8;
};

#endif // SYSTEM_EDITOR_WIDGET_H
