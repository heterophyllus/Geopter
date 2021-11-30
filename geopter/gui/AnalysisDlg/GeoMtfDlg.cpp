#include "GeoMtfDlg.h"
#include "ui_GeoMtfDlg.h"

GeoMtfDlg::GeoMtfDlg(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::GeoMtfDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Geometrical MTF");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    // sampling
    QStringList samplingComboItems;
    samplingComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512" << "1024x1024";
    ui->samplingCombo->clear();
    ui->samplingCombo->addItems(samplingComboItems);
    ui->samplingCombo->setCurrentIndex(1);

    // max frequency
    ui->maxFreqEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
    ui->maxFreqEdit->setText(QString::number(100));

}

GeoMtfDlg::~GeoMtfDlg()
{
    delete ui;
}

void GeoMtfDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();

    int nrd = 16 * pow(2, ui->samplingCombo->currentIndex());
    double maxFreq = ui->maxFreqEdit->text().toDouble();
    double step = 1.0;

    GeometricalMTF* geoMTF = new GeometricalMTF;
    auto plotData = geoMTF->plot(m_opticalSystem, nrd, maxFreq, step);
    delete geoMTF;

    m_renderer->clear();

    m_renderer->draw_plot(plotData);
    m_renderer->set_x_axis_range(0.0, maxFreq);
    m_renderer->set_y_axis_range(0.0, 1.0);
    m_renderer->set_x_axis_label("Frequency");
    m_renderer->set_y_axis_label("MTF");
    m_renderer->draw_x_axis();
    m_renderer->draw_y_axis();

    m_renderer->update();
}
