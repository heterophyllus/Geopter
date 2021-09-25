#include "longitudinal_setting_dialog.h"
#include "ui_longitudinal_setting_dialog.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LongitudinalSettingDialog::LongitudinalSettingDialog(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::LongitudinalSettingDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    setWindowTitle("Longitudinal Aberration Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->scaleEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->scaleEdit->setText(QString::number(0.3));
}

LongitudinalSettingDialog::~LongitudinalSettingDialog()
{
    delete m_renderer;
    delete ui;
}

void LongitudinalSettingDialog::updateParentDockContent()
{
    m_renderer->clear();

    double scale = ui->scaleEdit->text().toDouble();

    m_opticalSystem->update_model();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    abr->plot_longitudinal_spherical_aberration(scale);
    delete abr;

    m_renderer->update();
}

