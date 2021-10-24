#ifndef LONGITUDINAL_SETTING_DIALOG_H
#define LONGITUDINAL_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class LongitudinalDlg;
}

class LongitudinalDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit LongitudinalDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~LongitudinalDlg();

    void updateParentDockContent() override;

private:
    Ui::LongitudinalDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;

};

#endif // LONGITUDINAL_SETTING_DIALOG_H
