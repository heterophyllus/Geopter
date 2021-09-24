#ifndef CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
#define CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H

#include "Analysis/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "Analysis/renderer_qcp.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class ChromaticFocusShiftSettingDialog;
}

class ChromaticFocusShiftSettingDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit ChromaticFocusShiftSettingDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~ChromaticFocusShiftSettingDialog();

    void updateParentDockContent() override;

private:
    Ui::ChromaticFocusShiftSettingDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // CHROMATIC_FOCUSSHIFT_SETTING_DIALOG_H
