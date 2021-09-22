#ifndef TRANSVERSE_RAY_FAN_DIALOG_H
#define TRANSVERSE_RAY_FAN_DIALOG_H

#include "plot_view_dock.h"

namespace Ui {
class TransverseRayFanDialog;
}

class TransverseRayFanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransverseRayFanDialog(PlotViewDock *parent = nullptr);
    ~TransverseRayFanDialog();

    void getSettings(double *scale, int *nrd);

private:
    Ui::TransverseRayFanDialog *ui;
    PlotViewDock* parentDock_;
};

#endif // TRANSVERSE_RAY_FAN_DIALOG_H
