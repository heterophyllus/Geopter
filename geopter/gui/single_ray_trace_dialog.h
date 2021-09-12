#ifndef REAL_RAY_TRACE_DIALOG_H
#define REAL_RAY_TRACE_DIALOG_H

#include <QDialog>

#include "optical.h"
using namespace geopter;

class TextViewDock;

namespace Ui {
class SingleRayTraceDialog;
}


/** Setting dialog for Real Ray Trace */
class SingleRayTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingleRayTraceDialog(OpticalSystem* opt_sys, TextViewDock *parent = nullptr);
    ~SingleRayTraceDialog();

    void setOpticalSystem(OpticalSystem* opt_sys);

private slots:
    void showStackedTab(int i);

    void onAccept();

private:
    Ui::SingleRayTraceDialog *ui;
    TextViewDock *parentDock;

    OpticalSystem *opt_sys_;

    void doPupilRayTrace();
    void doObjectRayTrace();
};

#endif // REAL_RAY_TRACE_DIALOG_H
