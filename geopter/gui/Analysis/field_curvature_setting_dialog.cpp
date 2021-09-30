#include "field_curvature_setting_dialog.h"
#include "ui_field_curvature_setting_dialog.h"


#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

FieldCurvatureSettingDialog::FieldCurvatureSettingDialog(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::FieldCurvatureSettingDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->scaleEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->scaleEdit->setText(QString::number(0.3));

    ui->rayAimingCombo->addItems(QStringList({"Stop", "Entrance Pupil"}));
    ui->rayAimingCombo->setCurrentIndex(0);
}

FieldCurvatureSettingDialog::~FieldCurvatureSettingDialog()
{
    delete m_renderer;
    delete ui;
}

void FieldCurvatureSettingDialog::updateParentDockContent()
{
    m_renderer->clear();

    int rayAimingType = ui->rayAimingCombo->currentIndex();
    double scale = ui->scaleEdit->text().toDouble();

    m_opticalSystem->update_model();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    abr->plot_astigmatism(rayAimingType, scale);
    delete abr;

    m_renderer->update();
}
