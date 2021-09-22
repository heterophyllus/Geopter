#ifndef LONGITUDINAL_SETTING_DIALOG_H
#define LONGITUDINAL_SETTING_DIALOG_H

#include <QDialog>
#include "plot_view_dock.h"

namespace Ui {
class LongitudinalSettingDialog;
}

class LongitudinalSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LongitudinalSettingDialog(PlotViewDock *parent = nullptr);
    ~LongitudinalSettingDialog();

    double getScale();

public slots:
    void onAccept();

private:
    Ui::LongitudinalSettingDialog *ui;

    PlotViewDock * m_parentDock;

};

#endif // LONGITUDINAL_SETTING_DIALOG_H
