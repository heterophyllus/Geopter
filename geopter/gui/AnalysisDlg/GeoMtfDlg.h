#ifndef GEOMTFDLG_H
#define GEOMTFDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/PlotViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class GeoMtfDlg;
}

class GeoMtfDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit GeoMtfDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~GeoMtfDlg();

    void updateParentDockContent() override;

private:
    Ui::GeoMtfDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // GEOMTFDLG_H
