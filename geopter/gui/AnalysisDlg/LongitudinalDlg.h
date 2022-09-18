#ifndef LONGITUDINAL_SETTING_DIALOG_H
#define LONGITUDINAL_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"


namespace Ui {
class LongitudinalDlg;
}

class LongitudinalDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit LongitudinalDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~LongitudinalDlg();

    void updateParentDockContent() override;

private:
    Ui::LongitudinalDlg *ui;
    RendererQCP *m_renderer;
    AnalysisViewDock* m_parentDock;
};

#endif // LONGITUDINAL_SETTING_DIALOG_H
