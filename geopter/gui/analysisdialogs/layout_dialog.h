#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include "analysisdialogs/analysis_setting_dialog.h"
#include "plot_view_dock.h"
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class LayoutDialog;
}

class LayoutDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit LayoutDialog(OpticalSystem* sys, PlotViewDock *parent = nullptr);
    ~LayoutDialog();

    void updateParentDockContent() override;

private slots:
    //void onAccept();

private:
    Ui::LayoutDialog *ui;
    PlotViewDock* m_parentDock;
    RendererQCP *m_renderer;
    OpticalSystem *m_opticalSystem;
};

#endif // LAYOUT_DIALOG_H
