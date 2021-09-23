#ifndef LONGITUDINAL_SETTING_DIALOG_H
#define LONGITUDINAL_SETTING_DIALOG_H

#include "Analysis/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "Analysis/renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class LongitudinalSettingDialog;
}

class LongitudinalSettingDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit LongitudinalSettingDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~LongitudinalSettingDialog();

    void updateParentDockContent() override;

private:
    Ui::LongitudinalSettingDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;

};

#endif // LONGITUDINAL_SETTING_DIALOG_H
