#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class Layout2dDlg;
}

class Layout2dDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit Layout2dDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~Layout2dDlg();

    void updateParentDockContent() override;

private slots:
    void onAccept();

private:
    Ui::Layout2dDlg *ui;
    RendererQCP *m_renderer;
    AnalysisViewDock* m_parentDock;
};

#endif // LAYOUT_DIALOG_H
