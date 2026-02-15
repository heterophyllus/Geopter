#include "ChromaticFocusShiftDlg.h"

#include <ostream>

#include "ui_ChromaticFocusShiftDlg.h"

ChromaticFocusShiftDlg::ChromaticFocusShiftDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::ChromaticFocusShiftDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->minWvlEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 4, this));
    ui->maxWvlEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 4, this));

    double lower = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->LowerWavelength();
    ui->minWvlEdit->setText(QString::number(lower));
    double higher = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->HigherWavelength();
    ui->maxWvlEdit->setText(QString::number(higher));

}

ChromaticFocusShiftDlg::~ChromaticFocusShiftDlg()
{
    delete m_renderer;
    delete ui;
}

void ChromaticFocusShiftDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();

    double lower = ui->minWvlEdit->text().toDouble();
    double higher = ui->maxWvlEdit->text().toDouble();

    ChromaticFocusShift *chrom = new ChromaticFocusShift(m_opticalSystem);
    std::ostringstream oss;
    auto plotData = chrom->plot(lower, higher);
    plotData->Print(oss);
    m_parentDock->setText(oss);


    double lower_y, higher_y;
    plotData->GetGraph(0)->GetYRange(&lower_y, &higher_y);

    m_renderer->Clear();
    m_renderer->DrawPlot(plotData);
    m_renderer->SetXaxisRange(lower, higher);
    m_renderer->SetYaxisRange(lower_y, higher_y);
    m_renderer->SetXaxisLabel(plotData->XLabel());
    m_renderer->SetYaxisLabel(plotData->YLabel());
    m_renderer->DrawXaxis();
    m_renderer->DrawYaxis();
    m_renderer->Update();

    delete chrom;

    m_parentDock->setCurrentTab(1);

}
