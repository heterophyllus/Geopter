#include "Layout2dDlg.h"
#include "ui_Layout2dDlg.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

Layout2dDlg::Layout2dDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::Layout2dDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Layout Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->checkDrawRefRay->setChecked(true);
    ui->editNumRays->setValidator(new QIntValidator(1, 20, this));
    ui->editNumRays->setText(QString::number(10));

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Layout2dDlg::~Layout2dDlg()
{
    delete m_renderer;
    delete ui;
}


void Layout2dDlg::onAccept()
{
    //updateParentDockContent();
    accept();
}


void Layout2dDlg::updateParentDockContent()
{
    m_renderer->Clear();
    m_renderer->SetMouseInteraction(true);

    m_opticalSystem->UpdateModel();

    Layout *layout = new Layout(m_opticalSystem, m_renderer);

    layout->DrawElements();

    if(ui->checkDrawRefRay->checkState()){
        layout->DrawReferenceRays();
    }
    if(ui->checkDrawFan->checkState()){
        int nrd = ui->editNumRays->text().toInt();
        layout->DrawFanRays(nrd);
    }

    m_parentDock->setCurrentTab(1);
    layout->Update();

    delete layout;
}
