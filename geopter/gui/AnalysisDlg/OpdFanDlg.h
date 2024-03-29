#ifndef OPDFANDLG_H
#define OPDFANDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class OpdFanDlg;
}

class OpdFanDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit OpdFanDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~OpdFanDlg();

    void updateParentDockContent() override;

private:
    Ui::OpdFanDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // OPDFANDLG_H
