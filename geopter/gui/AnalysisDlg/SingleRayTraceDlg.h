#ifndef SINGLE_RAY_TRACE_DIALOG_H
#define SINGLE_RAY_TRACE_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/AnalysisViewDock.h"

namespace Ui {
class SingleRayTraceDlg;
}


/** Setting dialog for Single Ray Trace */
class SingleRayTraceDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit SingleRayTraceDlg(OpticalSystem *sys, AnalysisViewDock *parent = nullptr);
    ~SingleRayTraceDlg();

    void updateParentDockContent() override;

private slots:
    void showStackedTab(int i);

private:
    void doPupilRayTrace();
    void doObjectRayTrace();

    Ui::SingleRayTraceDlg *ui;
    AnalysisViewDock *m_parentDock;

};

#endif // REAL_RAY_TRACE_DIALOG_H
