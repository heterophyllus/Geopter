#include "LongitudinalDlg.h"
#include "ui_LongitudinalDlg.h"

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
    m_opticalSystem->UpdateModel();

    double scale = ui->scaleEdit->text().toDouble();
    int numRays = ui->numRaysEdit->text().toInt();

    Spherochromatism *sph = new Spherochromatism(m_opticalSystem);
    auto plotData = sph->plot(numRays);
    delete sph;

    std::ostringstream oss;
    plotData->Print(oss);

    m_parentDock->setText(oss);

    m_renderer->Clear();
    m_renderer->DrawPlot(plotData);
    m_renderer->SetXaxisRange(-scale, scale);
    m_renderer->SetYaxisRange(0.0, 1.0);
    m_renderer->SetXaxisLabel(plotData->XLabel());
    m_renderer->SetYaxisLabel(plotData->YLabel());
    m_renderer->DrawXaxis();
    m_renderer->DrawYaxis();
    m_renderer->Update();
}

