#ifndef CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
#define CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "PlotViewDock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class ChromaticFocusShiftDlg;
}

class ChromaticFocusShiftDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit ChromaticFocusShiftDlg(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~ChromaticFocusShiftDlg();

    void updateParentDockContent() override;

private:
    Ui::ChromaticFocusShiftDlg *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
