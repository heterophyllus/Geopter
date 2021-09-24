#include "transverse_ray_fan_dialog.h"
#include "ui_transverse_ray_fan_dialog.h"

#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"

TransverseRayFanDialog::TransverseRayFanDialog(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::TransverseRayFanDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Transverse Ray Fan Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->lineEdit_Scale->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 4, this));
    ui->lineEdit_Scale->setText(QString::number(0.05));
    ui->lineEdit_NRD->setValidator(new QIntValidator(1, 100, this));
    ui->lineEdit_NRD->setText(QString::number(20));
}

TransverseRayFanDialog::~TransverseRayFanDialog()
{
    delete m_renderer;
    delete ui;
}

void TransverseRayFanDialog::updateParentDockContent()
{
    m_renderer->clear();

    double scale = ui->lineEdit_Scale->text().toDouble();
    int nrd = ui->lineEdit_NRD->text().toInt();

    m_opticalSystem->update_model();

    RayFan *ray_fan = new RayFan(m_opticalSystem, m_renderer);
    ray_fan->plot(scale, nrd);
    delete ray_fan;

    m_renderer->update();
}
