#include "TransverseRayFanDlg.h"
#include "ui_transverserayfandlg.h"

#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"

TransverseRayFanDlg::TransverseRayFanDlg(OpticalSystem* sys, PlotViewDock *parent) :
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
    m_opticalSystem->update_model();
    int fieldCount = m_opticalSystem->optical_spec()->field_of_view()->field_count();
    double stopRadius = m_opticalSystem->optical_assembly()->stop_surface()->max_aperture();

    int ray_direction = ui->hAxisDataCombo->currentIndex();
    int abr_direction = ui->vAxisDataCombo->currentIndex();
    double scale = ui->scaleEdit->text().toDouble();
    int nrd = ui->nrdEdit->text().toInt();

    m_renderer->clear();
    m_renderer->set_grid_layout(fieldCount, 1);

    TransverseRayFan *ray_fan = new TransverseRayFan(m_opticalSystem);

    for(int fi = 0; fi < fieldCount; fi++){
        m_renderer->set_current_cell(fieldCount - fi - 1, 0);

        Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fi);
        auto plotData = ray_fan->plot(nrd, fld, ray_direction, abr_direction);

        m_renderer->draw_plot(plotData);
        m_renderer->set_x_axis_range(-stopRadius, stopRadius);
        m_renderer->set_y_axis_range(-scale, scale);
        m_renderer->set_x_axis_label(plotData->x_axis_label());
        m_renderer->set_y_axis_label(plotData->y_axis_label());
        m_renderer->draw_x_axis();
        m_renderer->draw_y_axis();
    }

    delete ray_fan;

    m_renderer->update();
}
