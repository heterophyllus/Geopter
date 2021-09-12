#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include <QDialog>

#include "optical.h"
using namespace geopter;

class TextViewDock;

namespace Ui {
class ParaxialTraceDialog;
}



class ParaxialTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParaxialTraceDialog(OpticalSystem* opt_sys, TextViewDock *parent = nullptr);
    ~ParaxialTraceDialog();

private slots:
    void onAccept();

private:
    Ui::ParaxialTraceDialog *ui;

    TextViewDock* parentDock_;
    OpticalSystem* opt_sys_;
};

#endif // PARAXIAL_TRACE_DIALOG_H
