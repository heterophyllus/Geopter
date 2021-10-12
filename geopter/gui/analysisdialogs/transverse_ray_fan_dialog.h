#ifndef TRANSVERSE_RAY_FAN_DIALOG_H
#define TRANSVERSE_RAY_FAN_DIALOG_H

#include "analysisdialogs/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class TransverseRayFanDialog;
}

class TransverseRayFanDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit TransverseRayFanDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~TransverseRayFanDialog();

    void updateParentDockContent() override;

private:
    Ui::TransverseRayFanDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // TRANSVERSE_RAY_FAN_DIALOG_H
