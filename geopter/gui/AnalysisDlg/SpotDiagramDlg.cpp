#include "SpotDiagramDlg.h"
#include "ui_spotdiagramdlg.h"

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
    m_opticalSystem->update_model();
    int fieldCount = m_opticalSystem->optical_spec()->field_of_view()->field_count();

    int pattern = ui->rayPatternCombo->currentIndex();
    int nrd = ui->nrdEdit->text().toInt();
    double scale = ui->scaleEdit->text().toDouble();
    double dotSize = ui->dotSizeEdit->text().toDouble();

    m_renderer->clear();
    m_renderer->set_grid_layout(fieldCount, 1);

    std::ostringstream oss;

    SpotDiagram *spot = new SpotDiagram(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++) {
        m_renderer->set_current_cell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fi);

        auto plotData = spot->plot(fld, pattern, nrd, dotSize);

        m_renderer->draw_plot(plotData);
        m_renderer->set_x_axis_range(-scale, scale);
        m_renderer->set_y_axis_range(-scale, scale);
        m_renderer->set_x_axis_label(plotData->x_axis_label());
        m_renderer->set_y_axis_label(plotData->y_axis_label());
        m_renderer->draw_x_axis();
        m_renderer->draw_y_axis();

        plotData->print(oss);
    }

    delete spot;

    m_renderer->update();

}
