#include "FieldCurvatureDlg.h"
#include "ui_FieldCurvatureDlg.h"


#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

FieldCurvatureDlg::FieldCurvatureDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::FieldCurvatureDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->scaleEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->scaleEdit->setText(QString::number(0.3));
    ui->numRaysEdit->setValidator(new QIntValidator(2, 100, this));
    ui->numRaysEdit->setText(QString::number(20));

    // FIXME : It is unclear why the buttonBox of this dialog itself doesn't work, though that of other dialogs work fine.
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

FieldCurvatureDlg::~FieldCurvatureDlg()
{
    delete m_renderer;
    delete ui;
}

void FieldCurvatureDlg::updateParentDockContent()
{
    m_opticalSystem->UpdateModel();
    double maxField = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->MaxField();

    double scale = ui->scaleEdit->text().toDouble();
    int numRays = ui->numRaysEdit->text().toInt();

    Astigmatism *ast = new Astigmatism(m_opticalSystem);
    auto plotData = ast->plot(numRays);

    std::ostringstream oss;
    plotData->Print(oss);

    m_renderer->Clear();
    m_renderer->DrawPlot(plotData);
    m_renderer->SetXaxisRange(-scale, scale);
    m_renderer->SetYaxisRange(0.0, maxField);
    m_renderer->SetXaxisLabel(plotData->XLabel());
    m_renderer->SetYaxisLabel(plotData->YLabel());
    m_renderer->DrawXaxis();
    m_renderer->DrawYaxis();
    m_renderer->Update();

    delete ast;

    m_parentDock->setText(oss);
}
