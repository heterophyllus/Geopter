#include "LongitudinalDlg.h"
#include "ui_longitudinaldlg.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LongitudinalDlg::LongitudinalDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::LongitudinalDlg),
    m_parentDock(parent)
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

    Spherochromatism *sph = new Spherochromatism(m_opticalSystem);
    auto plotData = sph->plot(numRays);
    delete sph;

    plotData->print();

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

