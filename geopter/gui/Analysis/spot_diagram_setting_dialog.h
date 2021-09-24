#ifndef SPOT_DIAGRAM_SETTING_DIALOG_H
#define SPOT_DIAGRAM_SETTING_DIALOG_H

#include "Analysis/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "Analysis/renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class SpotDiagramSettingDialog;
}

class SpotDiagramSettingDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit SpotDiagramSettingDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~SpotDiagramSettingDialog();

    void updateParentDockContent() override;

private:
    Ui::SpotDiagramSettingDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // SPOT_DIAGRAM_SETTING_DIALOG_H
