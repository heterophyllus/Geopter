#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "TextViewDock.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class ParaxialTraceDlg;
}

class ParaxialTraceDlg : public AnalysisSettingDlg
{
    Q_OBJECT

public:
    explicit ParaxialTraceDlg(OpticalSystem* sys, TextViewDock *parent = nullptr);
    ~ParaxialTraceDlg();

    void updateParentDockContent() override;

private:
    Ui::ParaxialTraceDlg *ui;
    TextViewDock* m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // PARAXIAL_TRACE_DIALOG_H
