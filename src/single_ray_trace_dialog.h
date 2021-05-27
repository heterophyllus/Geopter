#ifndef REAL_RAY_TRACE_DIALOG_H
#define REAL_RAY_TRACE_DIALOG_H

#include <QDialog>

#include "optical_core.h"
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
    explicit SingleRayTraceDialog(OpticalModel* opt_model, TextViewDock *parent = nullptr);
    ~SingleRayTraceDialog();

    void setOpticalModel(OpticalModel* opt_model);

private slots:
    void showStackedTab(int i);

    void onAccept();

private:
    Ui::SingleRayTraceDialog *ui;
    TextViewDock *parentDock;

    OpticalModel *opm;

    void doPupilRayTrace();
    void doObjectRayTrace();
};

#endif // REAL_RAY_TRACE_DIALOG_H
