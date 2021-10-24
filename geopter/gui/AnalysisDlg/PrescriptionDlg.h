#ifndef PRESCRIPTION_SETTING_DIALOG_H
#define PRESCRIPTION_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "TextViewDock.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class PrescriptionDlg;
}

class PrescriptionDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit PrescriptionDlg(OpticalSystem* sys, TextViewDock *parent = nullptr);
    ~PrescriptionDlg();

    void updateParentDockContent() override;

private:
    Ui::PrescriptionDlg *ui;
    TextViewDock* m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // PRESCRIPTION_SETTING_DIALOG_H
