#ifndef SPEC_SETTING_DIALOG_H
#define SPEC_SETTING_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QTableWidget>

#include "optical_core.h"
using namespace geopter;

namespace Ui {
class SpecSettingDialog;
}

// table column
enum WvlTab{
    WvlValue,
    WvlWeight,
    WvlColor
};

enum FieldTab{
    FldX,
    FldY,
    FldWeight,
    FldColor,
    FldVUY,
    FldVLY,
    FldVUX,
    FldVLX
};

class SpecSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpecSettingDialog(std::shared_ptr<OpticalModel> opt_model, QWidget *parent = nullptr);
    ~SpecSettingDialog();

    /** Apply optical spec properties for the model using UI input data  */
    void syncModelWithUi();

    /** Set up UI using current data of the optical model */
    void syncUiWithModel();

    void setOpticalModel(std::shared_ptr<OpticalModel> opt_model);

private slots:
    void addWvl();
    void removeWvl();

    void addField();
    void removeField();

    void onWvlCellDoubleClicked(int row, int col);
    void onFieldCellDoubleClicked(int row, int col);

    void buttonSetVignettingClicked();

    void onAccept();

private:
    Ui::SpecSettingDialog *ui;

    std::shared_ptr<OpticalModel> opt_model_;

    void setUpPupilTab();
    void setPupilData(const PupilSpec& pupil);

    void setUpWvlTab();
    void setWvlData(int row, const Wvl& wvl);
    void updateReferenceWvlCombo();

    void setUpFieldTab();
    void setFldData(int row, const Field& fld);

    void setValueToCell(QTableWidget* table, int row, int col, double val);
    void setValueToCell(QTableWidget* table, int row, int col, QString str);

    void setColorToCell(QTableWidget* table, int row, int col, QColor color);

    void setCellUneditable(QTableWidget* table, int row, int col);

    /** Open color pick dialog.  This function is called when 'color' cell is double clicked. */
    void selectWvlColor(int row, int col);

    /** Open color pick dialog.  This function is called when 'color' cell is double clicked. */
    void selectFldColor(int row, int col);

    /** Open wavelength input dialog.. This function is called when 'value' cell is double clicked. */
    void selectSpectralLine(int row, int col);

    QColor rgbToQColor(Rgb rgbColor);
    Rgb QColorToRgb(QColor color);
};

#endif // SPEC_SETTING_DIALOG_H
