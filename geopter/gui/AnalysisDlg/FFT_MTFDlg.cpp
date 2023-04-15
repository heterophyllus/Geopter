#include "FFT_MTFDlg.h"
#include "ui_FFT_MTFDlg.h"

FFT_MTFDlg::FFT_MTFDlg(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::FFT_MTFDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("FFT MTF");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    // sampling
    QStringList samplingComboItems;
    samplingComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512" << "1024x1024";
    ui->samplingCombo->clear();
    ui->samplingCombo->addItems(samplingComboItems);
    ui->samplingCombo->setCurrentIndex(3);

    // max frequency
    ui->maxFreqEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
    ui->maxFreqEdit->setText(QString::number(100));
}

FFT_MTFDlg::~FFT_MTFDlg()
{
    delete ui;
}

void FFT_MTFDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();

    int M = 16 * pow(2, ui->samplingCombo->currentIndex());
    double maxFreq = ui->maxFreqEdit->text().toDouble();

    DiffractiveMTF* mtf = new DiffractiveMTF(m_opticalSystem);
    auto plotData = mtf->plot(m_opticalSystem, M);
    delete mtf;

    std::ostringstream oss;
    plotData->Print(oss);
    std::cout << oss.str() << std::endl;

    m_renderer->Clear();

    m_renderer->DrawPlot(plotData);
    m_renderer->SetXaxisRange(0.0, maxFreq);
    m_renderer->SetYaxisRange(0.0, 1.0);
    m_renderer->SetXaxisLabel("Frequency");
    m_renderer->SetYaxisLabel("MTF");
    m_renderer->DrawXaxis();
    m_renderer->DrawYaxis();

    m_renderer->Update();

}
