#include "longitudinal_setting_dialog.h"
#include "ui_longitudinal_setting_dialog.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"
#include "plot_view_dock.h"
using namespace geopter;

LongitudinalSettingDialog::LongitudinalSettingDialog(OpticalModel* opt_model, PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::LongitudinalSettingDialog),
    parentDock_(parent),
    opt_model_(opt_model)
{
    ui->setupUi(this);

    this->setWindowTitle("Longitudinal Aberration Setting");

    ui->editLsaScale->setValidator(new QDoubleValidator(0.0, 1.0e+10, 4, this));
    ui->editLsaScale->setText(QString::number(0.1));
    ui->editAstScale->setValidator(new QDoubleValidator(0.0, 1.0e+10, 4, this));
    ui->editAstScale->setText(QString::number(0.1));

    renderer_ = new RendererQCP(parentDock_->customPlot());

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

}

LongitudinalSettingDialog::~LongitudinalSettingDialog()
{
    delete renderer_;
    delete ui;
}

void LongitudinalSettingDialog::onAccept()
{
    this->plot_all();
    accept();
}

void LongitudinalSettingDialog::plot_all()
{
    double lsa_scale = ui->editLsaScale->text().toDouble();
    double ast_scale = ui->editAstScale->text().toDouble();

    //opt_model_->update_model();

    Longitudinal lon(opt_model_, renderer_);
    lon.plot(lsa_scale, ast_scale);
}
