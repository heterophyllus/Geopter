#ifndef PRESCRIPTION_SETTING_DIALOG_H
#define PRESCRIPTION_SETTING_DIALOG_H

#include "analysisdialogs/analysis_setting_dialog.h"
#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class PrescriptionSettingDialog;
}

class PrescriptionSettingDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit PrescriptionSettingDialog(OpticalSystem* sys, TextViewDock *parent = nullptr);
    ~PrescriptionSettingDialog();

    void updateParentDockContent() override;

private:
    Ui::PrescriptionSettingDialog *ui;
    TextViewDock* m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // PRESCRIPTION_SETTING_DIALOG_H
