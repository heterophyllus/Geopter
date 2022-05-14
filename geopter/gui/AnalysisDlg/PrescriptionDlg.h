#ifndef PRESCRIPTION_SETTING_DIALOG_H
#define PRESCRIPTION_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/TextViewDock.h"

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
};

#endif // PRESCRIPTION_SETTING_DIALOG_H
