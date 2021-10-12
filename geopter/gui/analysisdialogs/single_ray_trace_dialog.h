#ifndef REAL_RAY_TRACE_DIALOG_H
#define REAL_RAY_TRACE_DIALOG_H

#include "analysisdialogs/analysis_setting_dialog.h"
#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class SingleRayTraceDialog;
}


/** Setting dialog for Single Ray Trace */
class SingleRayTraceDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit SingleRayTraceDialog(OpticalSystem *sys, TextViewDock *parent = nullptr);
    ~SingleRayTraceDialog();

    void updateParentDockContent() override;

private slots:
    void showStackedTab(int i);

private:
    void doPupilRayTrace();
    void doObjectRayTrace();

    Ui::SingleRayTraceDialog *ui;
    TextViewDock *m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // REAL_RAY_TRACE_DIALOG_H
