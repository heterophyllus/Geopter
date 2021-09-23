#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include "Analysis/analysis_setting_dialog.h"
#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class ParaxialTraceDialog;
}

class ParaxialTraceDialog : public AnalysisSettingDialog
{
    Q_OBJECT

public:
    explicit ParaxialTraceDialog(OpticalSystem* sys, TextViewDock *parent = nullptr);
    ~ParaxialTraceDialog();

    void updateParentDockContent() override;

private:
    Ui::ParaxialTraceDialog *ui;
    TextViewDock* m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // PARAXIAL_TRACE_DIALOG_H
