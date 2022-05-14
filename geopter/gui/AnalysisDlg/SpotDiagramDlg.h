#ifndef SPOT_DIAGRAM_SETTING_DIALOG_H
#define SPOT_DIAGRAM_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class SpotDiagramDlg;
}

class SpotDiagramDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit SpotDiagramDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~SpotDiagramDlg();

    void updateParentDockContent() override;

private:
    Ui::SpotDiagramDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // SPOT_DIAGRAM_SETTING_DIALOG_H
