#include "TransverseRayFanDlg.h"
#include "ui_TransverseRayFanDlg.h"

#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"

TransverseRayFanDlg::TransverseRayFanDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::TransverseRayFanDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Transverse Ray Fan Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->vAxisDataCombo->setCurrentIndex(1);
    ui->hAxisDataCombo->setCurrentIndex(1);
    ui->scaleEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 4, this));
    ui->scaleEdit->setText(QString::number(0.05));
    ui->nrdEdit->setValidator(new QIntValidator(1, 100, this));
    ui->nrdEdit->setText(QString::number(20));
}

TransverseRayFanDlg::~TransverseRayFanDlg()
{
    delete m_renderer;
    delete ui;
}

void TransverseRayFanDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();
    int fieldCount = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    double stopRadius = m_opticalSystem->GetOpticalAssembly()->StopSurface()->MaxAperture();

    int ray_direction = ui->hAxisDataCombo->currentIndex();
    int abr_direction = ui->vAxisDataCombo->currentIndex();
    double scale = ui->scaleEdit->text().toDouble();
    int nrd = ui->nrdEdit->text().toInt();

    m_renderer->Clear();
    m_renderer->SetGridLayout(fieldCount, 1);

    std::ostringstream oss;

    TransverseRayFan *ray_fan = new TransverseRayFan(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++){
        m_renderer->SetCurrentCell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fi);
        auto plotData = ray_fan->plot(nrd, fld, ray_direction, abr_direction);
        plotData->Print(oss);

        m_renderer->DrawPlot(plotData);
        m_renderer->SetXaxisRange(-stopRadius, stopRadius);
        m_renderer->SetYaxisRange(-scale, scale);
        m_renderer->SetXaxisLabel(plotData->XLabel());
        m_renderer->SetYaxisLabel(plotData->YLabel());
        m_renderer->DrawXaxis();
        m_renderer->DrawYaxis();
    }

    delete ray_fan;

    m_parentDock->setText(oss);

    m_renderer->Update();

    m_parentDock->setCurrentTab(1);
}
