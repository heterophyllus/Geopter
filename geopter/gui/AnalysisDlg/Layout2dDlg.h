#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class Layout2dDlg;
}

class Layout2dDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit Layout2dDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~Layout2dDlg();

    void updateParentDockContent() override;

private slots:
    //void onAccept();

private:
    Ui::Layout2dDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // LAYOUT_DIALOG_H
