#ifndef REAL_RAY_TRACE_DIALOG_H
#define REAL_RAY_TRACE_DIALOG_H

#include <QDialog>

#include "optical_core.h"
using namespace geopter;

class TextViewDock;

namespace Ui {
class RealRayTraceDialog;
}


/** Setting dialog for Real Ray Trace */
class RealRayTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RealRayTraceDialog(OpticalModel* opt_model, TextViewDock *parent = nullptr);
    ~RealRayTraceDialog();

    void setOpticalModel(OpticalModel* opt_model);

private slots:
    void showStackedTab(int i);

    void onAccept();

private:
    Ui::RealRayTraceDialog *ui;
    TextViewDock *parentDock;

    OpticalModel *opm;

    void doPupilRayTrace();
    void doObjectRayTrace();
};

#endif // REAL_RAY_TRACE_DIALOG_H
