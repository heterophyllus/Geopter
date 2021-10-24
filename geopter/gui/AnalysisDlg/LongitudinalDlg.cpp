#include "LongitudinalDlg.h"
#include "ui_longitudinaldlg.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LongitudinalDlg::LongitudinalDlg(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDlg(parent),
    ui(new Ui::LongitudinalDlg),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    setWindowTitle("Longitudinal Aberration Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->numRaysEdit->setValidator(new QIntValidator(2, 100, this));
    ui->numRaysEdit->setText(QString::number(20));
    ui->scaleEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->scaleEdit->setText(QString::number(0.3));
}

LongitudinalDlg::~LongitudinalDlg()
{
    delete m_renderer;
    delete ui;
}

void LongitudinalDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();

    double scale = ui->scaleEdit->text().toDouble();
    int numRays = ui->numRaysEdit->text().toInt();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    auto plotData = abr->plot_longitudinal_spherical_aberration(numRays);
    delete abr;

    m_renderer->clear();
    m_renderer->draw_plot(plotData);
    m_renderer->set_x_axis_range(-scale, scale);
    m_renderer->set_y_axis_range(0.0, 1.0);
    m_renderer->set_x_axis_label(plotData->x_axis_label());
    m_renderer->set_y_axis_label(plotData->y_axis_label());
    m_renderer->draw_x_axis();
    m_renderer->draw_y_axis();
    m_renderer->update();
}

