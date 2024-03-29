#ifndef SPOT_DIAGRAM_SETTING_DIALOG_H
#define SPOT_DIAGRAM_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class SpotDiagramDlg;
}

class SpotDiagramDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit SpotDiagramDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~SpotDiagramDlg();

    void updateParentDockContent() override;

private:
    Ui::SpotDiagramDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // SPOT_DIAGRAM_SETTING_DIALOG_H
