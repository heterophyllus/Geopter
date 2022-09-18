#ifndef PRESCRIPTION_SETTING_DIALOG_H
#define PRESCRIPTION_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"

namespace Ui {
class PrescriptionDlg;
}

class PrescriptionDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit PrescriptionDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~PrescriptionDlg();

    void updateParentDockContent() override;

private:
    Ui::PrescriptionDlg *ui;
    AnalysisViewDock* m_parentDock;
};

#endif // PRESCRIPTION_SETTING_DIALOG_H
