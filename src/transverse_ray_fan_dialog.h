#ifndef TRANSVERSE_RAY_FAN_DIALOG_H
#define TRANSVERSE_RAY_FAN_DIALOG_H

#include <QDialog>
#include "renderer_qcp.h"
#include "optical_core.h"
using namespace geopter;

class PlotViewDock;

namespace Ui {
class TransverseRayFanDialog;
}

class TransverseRayFanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransverseRayFanDialog(OpticalModel* opt_model, PlotViewDock *parent = nullptr);
    ~TransverseRayFanDialog();

private slots:
    void onAccept();

private:
    Ui::TransverseRayFanDialog *ui;
    PlotViewDock* parentDock_;
    OpticalModel* opm_;

    RendererQCP *renderer_;

    void plotRayFan();
};

#endif // TRANSVERSE_RAY_FAN_DIALOG_H
