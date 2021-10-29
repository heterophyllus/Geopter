#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

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
    RendererQCP *m_renderer;
    PlotViewDock* m_parentDock;
};

#endif // LAYOUT_DIALOG_H
