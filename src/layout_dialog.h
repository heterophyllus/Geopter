#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include <QDialog>
#include "renderer_qcp.h"
#include "optical_core.h"
using namespace geopter;

class PlotViewDock;

namespace Ui {
class LayoutDialog;
}

class LayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LayoutDialog(OpticalModel* opt_model, PlotViewDock *parent = nullptr);
    ~LayoutDialog();

private slots:
    void onAccept();

private:
    Ui::LayoutDialog *ui;

    PlotViewDock* parentDock_;
    OpticalModel* opt_model_;

    RendererQCP *renderer_;

    void drawElements();
    void drawRay();

};

#endif // LAYOUT_DIALOG_H
