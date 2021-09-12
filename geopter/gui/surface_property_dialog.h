#ifndef SURFACE_PROPERTY_DIALOG_H
#define SURFACE_PROPERTY_DIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class SurfacePropertyDialog;
}

#include "optical.h"
using namespace geopter;

class SurfacePropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurfacePropertyDialog(std::shared_ptr<OpticalSystem> opt_model, int surface_index, QWidget *parent = nullptr);
    ~SurfacePropertyDialog();

    void syncUiWithModel();
    void syncModelWithUi();

private slots:

    /** Show stacked property tab.  Called when the list item is changed  */
    void showStackedPropertyTab(int i);


    void initializeCoefsTable(int type);

    /** Show stacked aperture setting tab.  Called when the combobox(Aperture Type) index is changed */
    void showStackedApertureTab(int i);

    /** OK button clicked */
    void onAccept();

private:
    Ui::SurfacePropertyDialog *ui;
    std::shared_ptr<OpticalSystem> opt_sys_;
    int surface_index_;


    void setValueToCell(QTableWidget* table, int row, int col, double val);
};

#endif // SURFACE_PROPERTY_DIALOG_H
