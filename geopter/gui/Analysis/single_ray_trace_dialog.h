#ifndef REAL_RAY_TRACE_DIALOG_H
#define REAL_RAY_TRACE_DIALOG_H

#include <QDialog>
#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;

namespace Ui {
class SingleRayTraceDialog;
}


/** Setting dialog for Single Ray Trace */
class SingleRayTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingleRayTraceDialog(OpticalSystem *sys, TextViewDock *parent = nullptr);
    ~SingleRayTraceDialog();

    int getTraceType() const;
    void getSettingsForPupilRayTrace(double *px, double *py, int *fi, int *wi) const;
    void getSettingsForObjectRayTrace(double *x, double *y, double *tanX, double *tanY, int *wi) const;

private slots:
    void showStackedTab(int i);

private:
    Ui::SingleRayTraceDialog *ui;
    TextViewDock *m_parentDock;
    OpticalSystem* m_opticalSystem;
};

#endif // REAL_RAY_TRACE_DIALOG_H
