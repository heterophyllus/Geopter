#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include <QDialog>
#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;


namespace Ui {
class ParaxialTraceDialog;
}

class ParaxialTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParaxialTraceDialog(OpticalSystem* opt_sys, TextViewDock *parent = nullptr);
    ~ParaxialTraceDialog();

    void getSettings(int *wi);

private:
    Ui::ParaxialTraceDialog *ui;
    TextViewDock* m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // PARAXIAL_TRACE_DIALOG_H
