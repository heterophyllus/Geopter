#ifndef LONGITUDINAL_SETTING_DIALOG_H
#define LONGITUDINAL_SETTING_DIALOG_H

#include <QDialog>
#include "renderer_qcp.h"
#include "optical_core.h"

class PlotViewDock;

namespace Ui {
class LongitudinalSettingDialog;
}

class LongitudinalSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LongitudinalSettingDialog(OpticalModel* opt_model, PlotViewDock *parent = nullptr);
    ~LongitudinalSettingDialog();

private slots:
    void onAccept();

private:
    Ui::LongitudinalSettingDialog *ui;
    PlotViewDock* parentDock_;
    OpticalModel* opt_model_;

    RendererQCP *renderer_;

    void plot_all();
};

#endif // LONGITUDINAL_SETTING_DIALOG_H
