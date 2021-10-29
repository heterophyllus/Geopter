#include "Layout2dDlg.h"
#include "ui_layout2ddlg.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

Layout2dDlg::Layout2dDlg(OpticalSystem* sys, PlotViewDock *parent) :
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

    //QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

Layout2dDlg::~Layout2dDlg()
{
    delete m_renderer;
    delete ui;
}

/*
void LayoutDialog::onAccept()
{
    //updateParentDockContent();
    accept();
}
*/

void Layout2dDlg::updateParentDockContent()
{
    m_renderer->clear();
    m_renderer->set_mouse_interaction(true);

    m_opticalSystem->update_model();

    Layout *layout = new Layout(m_opticalSystem, m_renderer);

    layout->draw_elements();

    if(ui->checkDrawRefRay->checkState()){
        layout->draw_reference_rays();
    }
    if(ui->checkDrawFan->checkState()){
        int nrd = ui->editNumRays->text().toInt();
        layout->draw_fan_rays(nrd);
    }

    layout->update();

    delete layout;
}
