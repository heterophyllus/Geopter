#ifndef FIELD_CURVATURE_SETTING_DIALOG_H
#define FIELD_CURVATURE_SETTING_DIALOG_H

#include "analysisdialogs/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class FieldCurvatureSettingDialog;
}

class FieldCurvatureSettingDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit FieldCurvatureSettingDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~FieldCurvatureSettingDialog();

    void updateParentDockContent() override;

private:
    Ui::FieldCurvatureSettingDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // FIELD_CURVATURE_SETTING_DIALOG_H
