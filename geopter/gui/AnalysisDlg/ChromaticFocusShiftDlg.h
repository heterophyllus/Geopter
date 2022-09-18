#ifndef CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
#define CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"
#include "renderer_qcp.h"

namespace Ui {
class ChromaticFocusShiftDlg;
}

class ChromaticFocusShiftDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit ChromaticFocusShiftDlg(OpticalSystem* sys, AnalysisViewDock *parent = nullptr);
    ~ChromaticFocusShiftDlg();

    void updateParentDockContent() override;

private:
    Ui::ChromaticFocusShiftDlg *ui;
    AnalysisViewDock* m_parentDock;
    RendererQCP *m_renderer;

};

#endif // CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
