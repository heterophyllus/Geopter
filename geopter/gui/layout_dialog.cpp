#include "layout_dialog.h"
#include "ui_layout_dialog.h"

#include "renderer_qcp.h"
#include "plot_view_dock.h"

#include <fstream>

LayoutDialog::LayoutDialog(OpticalSystem* opt_sys, PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::LayoutDialog),
    parentDock_(parent),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Layout Setting");

    renderer_ = new RendererQCP(parentDock_->customPlot());

    ui->checkDrawRefRay->setChecked(true);
    ui->editNumRays->setValidator(new QIntValidator(1, 20, this));

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

}

LayoutDialog::~LayoutDialog()
{
    delete renderer_;
    delete ui;
}


void LayoutDialog::onAccept()
{
    renderer_->clear();

    opt_sys_->update_model();
    //opt_sys_->elem_model()->elements_from_sequence();

    Layout *layout = new Layout(opt_sys_, renderer_);

    layout->draw_elements();

    if(ui->checkDrawRefRay->checkState()){
        layout->draw_reference_rays();
    }
    if(ui->checkDrawFan->checkState()){
        int nrd = ui->editNumRays->text().toInt();
        layout->draw_fan_rays(nrd);
    }

    layout->update();

    delete layout;

    accept();
}
