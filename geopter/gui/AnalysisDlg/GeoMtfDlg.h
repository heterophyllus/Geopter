#ifndef GEOMTFDLG_H
#define GEOMTFDLG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class GeoMtfDlg;
}

class GeoMtfDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit GeoMtfDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~GeoMtfDlg();

    void updateParentDockContent() override;

private:
    Ui::GeoMtfDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;
};

#endif // GEOMTFDLG_H
