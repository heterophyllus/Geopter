#ifndef SURFACE_PROPERTY_DIALOG_H
#define SURFACE_PROPERTY_DIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class SurfacePropertyDlg;
}

#include "optical.h"
using namespace geopter;

class SurfacePropertyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SurfacePropertyDlg(std::shared_ptr<OpticalSystem> opt_sys, int surfaceIndex, QWidget *parent = nullptr);
    ~SurfacePropertyDlg();

    /** Update UI using current OpticalSystem */
    void syncUiWithSystem();

    /** Update OpticalSystem using current input data */
    void syncSystemWithUi();

private slots:

    /** Set up and show surface profile setting. Called when comboBox(Surafce Profile Type) is changed */
    void showSurfaceProfileStackPage(int surfaceProfileType);

    /** Show the given page of aperture stack.  Called when the combobox(Aperture Type) index is changed */
    void showApertureStackPage(int apertureType);

    /** OK button clicked */
    void onAccept();

private:
    void initializeEvenAsphereDataTable();
    void initializeOddAsphereDataTable();
    void setValueToCell(QTableWidget* table, int row, int col, double val);

    Ui::SurfacePropertyDlg *ui;
    std::shared_ptr<OpticalSystem> opt_sys_;
    int surface_index_;
};

#endif // SURFACE_PROPERTY_DIALOG_H
