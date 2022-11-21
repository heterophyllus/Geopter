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
    m_opticalSystem->update_model();
    int fieldCount = m_opticalSystem->optical_spec()->field_of_view()->field_count();

    double scale = ui->scaleEdit->text().toDouble();
    int nrd = 65;

    m_renderer->clear();
    m_renderer->set_grid_layout(fieldCount, 1);

    std::ostringstream oss;

    OpdFan *opd_fan = new OpdFan(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++){
        m_renderer->set_current_cell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fi);
        auto plotData = opd_fan->plot(fld, nrd);

        plotData->print(oss);

        m_renderer->draw_plot(plotData);
        m_renderer->set_x_axis_range(-1.0, 1.0);
        m_renderer->set_y_axis_range(-scale, scale);
        m_renderer->set_x_axis_label(plotData->x_axis_label());
        m_renderer->set_y_axis_label(plotData->y_axis_label());
        m_renderer->draw_x_axis();
        m_renderer->draw_y_axis();
    }

    delete opd_fan;

    m_renderer->update();

    m_parentDock->setText(oss);
    m_parentDock->setCurrentTab(1);

}
