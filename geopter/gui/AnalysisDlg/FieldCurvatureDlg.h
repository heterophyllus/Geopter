#ifndef FIELD_CURVATURE_SETTING_DIALOG_H
#define FIELD_CURVATURE_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class FieldCurvatureDlg;
}

class FieldCurvatureDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit FieldCurvatureDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~FieldCurvatureDlg();

    void updateParentDockContent() override;

private:
    Ui::FieldCurvatureDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // FIELD_CURVATURE_SETTING_DIALOG_H
