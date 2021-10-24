#include "ChromaticFocusShiftDlg.h"
#include "ui_chromaticfocusshiftdlg.h"

ChromaticFocusShiftDlg::ChromaticFocusShiftDlg(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDlg(parent),
    ui(new Ui::ChromaticFocusShiftDlg),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->minWvlEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 4, this));
    ui->maxWvlEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 4, this));

    double lower = m_opticalSystem->optical_spec()->spectral_region()->lower_wavelength();
    ui->minWvlEdit->setText(QString::number(lower));
    double higher = m_opticalSystem->optical_spec()->spectral_region()->higher_wavelength();
    ui->maxWvlEdit->setText(QString::number(higher));

}

ChromaticFocusShiftDlg::~ChromaticFocusShiftDlg()
{
    delete m_renderer;
    delete ui;
}

void ChromaticFocusShiftDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();

    double lower = ui->minWvlEdit->text().toDouble();
    double higher = ui->maxWvlEdit->text().toDouble();

    Aberration *abr = new Aberration(m_opticalSystem, m_renderer);
    auto plotData = abr->plot_chromatic_focus_shift(lower, higher);


    double higher_y = plotData->point_set(0)->higher_y();
    double lower_y  = plotData->point_set(0)->lower_y();

    m_renderer->clear();
    m_renderer->draw_plot(plotData);
    m_renderer->set_x_axis_range(lower, higher);
    m_renderer->set_y_axis_range(lower_y, higher_y);
    m_renderer->set_x_axis_label(plotData->x_axis_label());
    m_renderer->set_y_axis_label(plotData->y_axis_label());
    m_renderer->draw_x_axis();
    m_renderer->draw_y_axis();
    m_renderer->update();

    delete abr;
}
