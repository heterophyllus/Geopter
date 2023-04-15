#include "OpdFanDlg.h"
#include "ui_OpdFanDlg.h"

OpdFanDlg::OpdFanDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::OpdFanDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("OPD Fan Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->scaleEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 4, this));
    ui->scaleEdit->setText(QString::number(5.0));
}

OpdFanDlg::~OpdFanDlg()
{
    delete ui;
}

void OpdFanDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();
    int fieldCount = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();

    double scale = ui->scaleEdit->text().toDouble();
    int nrd = 65;

    m_renderer->Clear();
    m_renderer->SetGridLayout(fieldCount, 1);

    std::ostringstream oss;

    OpdFan *opd_fan = new OpdFan(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++){
        m_renderer->SetCurrentCell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fi);
        auto plotData = opd_fan->plot(fld, nrd);

        plotData->Print(oss);

        m_renderer->DrawPlot(plotData);
        m_renderer->SetXaxisRange(-1.0, 1.0);
        m_renderer->SetYaxisRange(-scale, scale);
        m_renderer->SetXaxisLabel(plotData->XLabel());
        m_renderer->SetYaxisLabel(plotData->YLabel());
        m_renderer->DrawXaxis();
        m_renderer->DrawYaxis();
    }

    delete opd_fan;

    m_renderer->Update();

    m_parentDock->setText(oss);
    m_parentDock->setCurrentTab(1);

}
