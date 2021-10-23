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
    ui->numRaysEdit->setValidator(new QIntValidator(2, 100, this));
    ui->numRaysEdit->setText(QString::number(20));

    ui->rayAimingCombo->addItems(QStringList({"Chief Ray", "Intermediate Ray"}));
    ui->rayAimingCombo->setCurrentIndex(0);
}

FieldCurvatureSettingDialog::~FieldCurvatureSettingDialog()
{
    delete m_renderer;
    delete ui;
}

void FieldCurvatureSettingDialog::updateParentDockContent()
{
    m_opticalSystem->update_model();
    double maxField = m_opticalSystem->optical_spec()->field_of_view()->max_field();

    int rayAimingType = ui->rayAimingCombo->currentIndex();
    double scale = ui->scaleEdit->text().toDouble();
    int numRays = ui->numRaysEdit->text().toInt();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    auto plotData = abr->plot_astigmatism(rayAimingType, numRays);

    plotData->print();

    m_renderer->clear();
    m_renderer->draw_plot(plotData);
    m_renderer->set_x_axis_range(-scale, scale);
    m_renderer->set_y_axis_range(0.0, maxField);
    m_renderer->set_x_axis_label(plotData->x_axis_label());
    m_renderer->set_y_axis_label(plotData->y_axis_label());
    m_renderer->draw_x_axis();
    m_renderer->draw_y_axis();
    m_renderer->update();

    delete abr;
}
