#ifndef SINGLE_RAY_TRACE_DIALOG_H
#define SINGLE_RAY_TRACE_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "Dock/TextViewDock.h"

namespace Ui {
class SingleRayTraceDlg;
}


/** Setting dialog for Single Ray Trace */
class SingleRayTraceDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit SingleRayTraceDlg(OpticalSystem *sys, TextViewDock *parent = nullptr);
    ~SingleRayTraceDlg();

    void updateParentDockContent() override;

private slots:
    void showStackedTab(int i);

private:
    void doPupilRayTrace();
    void doObjectRayTrace();

    Ui::SingleRayTraceDlg *ui;
    TextViewDock *m_parentDock;

};

#endif // REAL_RAY_TRACE_DIALOG_H
