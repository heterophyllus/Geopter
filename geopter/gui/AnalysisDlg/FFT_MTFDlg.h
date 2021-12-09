#ifndef FFT_MTFDLG_H
#define FFT_MTFDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class FFT_MTFDlg;
}

class FFT_MTFDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit FFT_MTFDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~FFT_MTFDlg();

    void updateParentDockContent() override;

private:
    Ui::FFT_MTFDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // FFT_MTFDLG_H
