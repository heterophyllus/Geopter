#ifndef WAVEFRONTMAPDLG_H
#define WAVEFRONTMAPDLG_H


#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class WavefrontMapDlg;
}

class WavefrontMapDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit WavefrontMapDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~WavefrontMapDlg();

    void updateParentDockContent() override;

private:
    Ui::WavefrontMapDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // WAVEFRONTMAPDLG_H
