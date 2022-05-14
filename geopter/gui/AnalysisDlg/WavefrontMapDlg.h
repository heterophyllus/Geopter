#ifndef WAVEFRONTMAPDLG_H
#define WAVEFRONTMAPDLG_H


#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class WavefrontMapDlg;
}

class WavefrontMapDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit WavefrontMapDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~WavefrontMapDlg();

    void updateParentDockContent() override;

private:
    Ui::WavefrontMapDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // WAVEFRONTMAPDLG_H
