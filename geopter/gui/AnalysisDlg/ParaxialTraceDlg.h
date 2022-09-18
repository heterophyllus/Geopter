#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"

namespace Ui {
class ParaxialTraceDlg;
}

class ParaxialTraceDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit ParaxialTraceDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~ParaxialTraceDlg();

    void updateParentDockContent() override;

private:
    Ui::ParaxialTraceDlg *ui;
    AnalysisViewDock* m_parentDock;
};

#endif // PARAXIAL_TRACE_DIALOG_H
