#include "layout_dialog.h"
#include "ui_layout_dialog.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LayoutDialog::LayoutDialog(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::LayoutDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Layout Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->checkDrawRefRay->setChecked(true);
    ui->editNumRays->setValidator(new QIntValidator(1, 20, this));
    ui->editNumRays->setText(QString::number(10));

    //QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

LayoutDialog::~LayoutDialog()
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

void LayoutDialog::updateParentDockContent()
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
