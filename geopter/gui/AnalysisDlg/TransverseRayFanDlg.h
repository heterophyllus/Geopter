#ifndef TRANSVERSE_RAY_FAN_DIALOG_H
#define TRANSVERSE_RAY_FAN_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class TransverseRayFanDlg;
}

class TransverseRayFanDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit TransverseRayFanDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~TransverseRayFanDlg();

    void updateParentDockContent() override;

private:
    Ui::TransverseRayFanDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // TRANSVERSE_RAY_FAN_DIALOG_H
