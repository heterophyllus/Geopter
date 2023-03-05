#ifndef FFT_PSFDLG_H
#define FFT_PSFDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class FFT_PSFDlg;
}

class FFT_PSFDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit FFT_PSFDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~FFT_PSFDlg();

    void updateParentDockContent() override;

private:
    Ui::FFT_PSFDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // FFT_PSFDLG_H
