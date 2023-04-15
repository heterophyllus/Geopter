#include "GeoMtfDlg.h"
#include "ui_GeoMtfDlg.h"

GeoMtfDlg::GeoMtfDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
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
    m_opticalSystem->UpdateModel();

    int nrd = 16 * pow(2, ui->samplingCombo->currentIndex());
    double maxFreq = ui->maxFreqEdit->text().toDouble();
    double step = 1.0;

    std::ostringstream oss;

    GeometricalMTF* geoMTF = new GeometricalMTF;
    auto plotData = geoMTF->plot(m_opticalSystem, nrd, maxFreq, step);
    plotData->Print(oss);
    delete geoMTF;

    m_renderer->Clear();

    m_renderer->DrawPlot(plotData);
    m_renderer->SetXaxisRange(0.0, maxFreq);
    m_renderer->SetYaxisRange(0.0, 1.0);
    m_renderer->SetXaxisLabel("Frequency");
    m_renderer->SetYaxisLabel("MTF");
    m_renderer->DrawXaxis();
    m_renderer->DrawYaxis();

    m_renderer->Update();

    m_parentDock->setText(oss);

    m_parentDock->setCurrentTab(1);


}
