#ifndef SINGLE_RAY_TRACE_DIALOG_H
#define SINGLE_RAY_TRACE_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "TextViewDock.h"

#include "optical.h"
using namespace geopter;

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
    OpticalSystem* m_opticalSystem;
};

#endif // REAL_RAY_TRACE_DIALOG_H
