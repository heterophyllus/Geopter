#include "spot_diagram_setting_dialog.h"
#include "ui_spot_diagram_setting_dialog.h"

#include <QLineEdit>
#include <QValidator>

SpotDiagramSettingDialog::SpotDiagramSettingDialog(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::SpotDiagramSettingDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());
    ui->nrdEdit->setValidator(new QIntValidator(1,1000,this));
    ui->nrdEdit->setText(QString::number(30));
    ui->scaleEdit->setValidator(new QDoubleValidator(0.0,1000.0,4,this));
    ui->scaleEdit->setText(QString::number(0.1));
    ui->dotSizeEdit->setValidator(new QDoubleValidator(0.0,1000.0,4,this));
    ui->dotSizeEdit->setText(QString::number(0.5));
}

SpotDiagramSettingDialog::~SpotDiagramSettingDialog()
{
    delete m_renderer;
    delete ui;
}

void SpotDiagramSettingDialog::updateParentDockContent()
{
    m_renderer->clear();

    int nrd = ui->nrdEdit->text().toInt();
    double scale = ui->scaleEdit->text().toDouble();
    double dotSize = ui->dotSizeEdit->text().toDouble();

    m_opticalSystem->update_model();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    abr->plot_spot_diagram(nrd,scale,dotSize);
    delete abr;

    m_renderer->update();

}
