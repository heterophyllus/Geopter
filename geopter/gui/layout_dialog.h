#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include <QDialog>
#include "renderer_qcp.h"

#include "optical.h"
using namespace geopter;

class PlotViewDock;

namespace Ui {
class LayoutDialog;
}

class LayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LayoutDialog(OpticalSystem* opt_sys, PlotViewDock *parent = nullptr);
    ~LayoutDialog();

private slots:
    void onAccept();

private:
    Ui::LayoutDialog *ui;

    PlotViewDock* parentDock_;
    OpticalSystem* opt_sys_;

    RendererQCP *renderer_;

    void drawElements();
    void drawRay();

};

#endif // LAYOUT_DIALOG_H
