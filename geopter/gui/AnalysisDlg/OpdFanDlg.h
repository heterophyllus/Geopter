#ifndef OPDFANDLG_H
#define OPDFANDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class OpdFanDlg;
}

class OpdFanDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit OpdFanDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~OpdFanDlg();

    void updateParentDockContent() override;

private:
    Ui::OpdFanDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // OPDFANDLG_H
