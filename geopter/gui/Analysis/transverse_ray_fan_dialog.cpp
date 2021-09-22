#include "transverse_ray_fan_dialog.h"
#include "ui_transverse_ray_fan_dialog.h"

#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"
#include "plot_view_dock.h"
using namespace geopter;

TransverseRayFanDialog::TransverseRayFanDialog(PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::TransverseRayFanDialog),
    parentDock_(parent)
{
    ui->setupUi(this);

    this->setWindowTitle("Transverse Ray Fan Setting");

    ui->lineEdit_Scale->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 4, this));
    ui->lineEdit_Scale->setText(QString::number(0.05));
    ui->lineEdit_NRD->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_NRD->setText(QString::number(20));
}

TransverseRayFanDialog::~TransverseRayFanDialog()
{
    delete ui;
}

void TransverseRayFanDialog::getSettings(double *scale, int *nrd)
{
    *scale = ui->lineEdit_Scale->text().toDouble();
    *nrd = ui->lineEdit_NRD->text().toDouble();
}
