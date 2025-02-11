#include "SpotDiagramDlg.h"
#include "ui_SpotDiagramDlg.h"

#include <iostream>
#include <QLineEdit>
#include <QValidator>

SpotDiagramDlg::SpotDiagramDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys,parent),
    ui(new Ui::SpotDiagramDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->rayPatternCombo->clear();
    ui->rayPatternCombo->addItems(QStringList({"Grid", "Hexapolar"}));
    ui->rayPatternCombo->setCurrentIndex(0);
    ui->nrdEdit->setValidator(new QIntValidator(1,1000,this));
    ui->nrdEdit->setText(QString::number(20));
    ui->scaleEdit->setValidator(new QDoubleValidator(0.0,1000.0,4,this));
    ui->scaleEdit->setText(QString::number(0.1));
    ui->dotSizeEdit->setValidator(new QDoubleValidator(0.0,1000.0,4,this));
    ui->dotSizeEdit->setText(QString::number(0.5));
}

SpotDiagramDlg::~SpotDiagramDlg()
{
    delete m_renderer;
    delete ui;
}

void SpotDiagramDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();
    int fieldCount = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();

    int pattern = ui->rayPatternCombo->currentIndex();
    int nrd = ui->nrdEdit->text().toInt();
    double scale = ui->scaleEdit->text().toDouble();
    double dotSize = ui->dotSizeEdit->text().toDouble();

    m_renderer->Clear();
    m_renderer->SetGridLayout(fieldCount, 1);

    std::ostringstream oss;

    SpotDiagram *spot = new SpotDiagram(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++) {
        m_renderer->SetCurrentCell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fi);

        auto plotData = spot->plot(fld, pattern, nrd, dotSize);

        m_renderer->DrawPlot(plotData);
        m_renderer->SetXaxisRange(-scale, scale);
        m_renderer->SetYaxisRange(-scale, scale);
        m_renderer->SetXaxisLabel(plotData->XLabel());
        m_renderer->SetYaxisLabel(plotData->YLabel());
        m_renderer->DrawXaxis();
        m_renderer->DrawYaxis();

        plotData->Print(oss);
    }

    delete spot;

    m_renderer->Update();

}
