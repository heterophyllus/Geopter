#ifndef TRANSVERSE_RAY_FAN_DIALOG_H
#define TRANSVERSE_RAY_FAN_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class TransverseRayFanDlg;
}

class TransverseRayFanDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit TransverseRayFanDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~TransverseRayFanDlg();

    void updateParentDockContent() override;

private:
    Ui::TransverseRayFanDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // TRANSVERSE_RAY_FAN_DIALOG_H
