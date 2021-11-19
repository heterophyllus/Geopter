#include "FFT_PSFDlg.h"
#include "ui_FFT_PSFDlg.h"

FFT_PSFDlg::FFT_PSFDlg(OpticalSystem* sys, PlotViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::FFT_PSFDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("FFT PSF");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    // sampling
    QStringList samplingComboItems;
    samplingComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512";
    ui->samplingCombo->clear();
    ui->samplingCombo->addItems(samplingComboItems);
    ui->samplingCombo->setCurrentIndex(3);

    // display
    QStringList displayComboItems;
    displayComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512";
    ui->displayCombo->clear();
    ui->displayCombo->addItems(displayComboItems);
    ui->displayCombo->setCurrentIndex(2);

    // field
    QStringList fieldComboItems;
    int numField = m_opticalSystem->optical_spec()->field_of_view()->field_count();
    for(int fi = 0; fi < numField; fi ++) {
        fieldComboItems << QString::number(fi);
    }
    ui->fieldCombo->addItems(fieldComboItems);
    ui->fieldCombo->setCurrentIndex(0);

    // wavelength
    QStringList wvlComboItems;
    int numWvl = m_opticalSystem->optical_spec()->spectral_region()->wvl_count();
    for(int wi = 0; wi < numWvl; wi++) {
        wvlComboItems << QString::number(wi);
    }
    ui->wvlCombo->addItems(wvlComboItems);
    ui->wvlCombo->setCurrentIndex(0);
}

FFT_PSFDlg::~FFT_PSFDlg()
{
    delete ui;
}

void FFT_PSFDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();
    int fieldIndex = ui->fieldCombo->currentIndex();
    int wvlIndex = ui->wvlCombo->currentIndex();

    int ndim = 16 * pow(2, ui->samplingCombo->currentIndex());
    int dispdim = 16 * pow(2, ui->displayCombo->currentIndex());

    m_renderer->clear();
    m_renderer->set_x_axis_range(-1.0, 1.0);
    m_renderer->set_y_axis_range(-1.0, 1.0);

    DiffractivePSF* psf = new DiffractivePSF(m_opticalSystem);
    Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fieldIndex);
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wvlIndex)->value();

    auto mapdata = psf->plot(fld, wvl, ndim, ndim*2);

    m_renderer->draw_colored_map(mapdata);

    double ratio = static_cast<double>(dispdim)/static_cast<double>(ndim);
    ratio = ratio*ratio;

    double currentXmin = m_renderer->current_x_axis_range().lower;
    double currentXmax = m_renderer->current_x_axis_range().upper;
    double currentYmin = m_renderer->current_y_axis_range().lower;
    double currentYmax = m_renderer->current_y_axis_range().upper;


    m_renderer->set_x_axis_range(currentXmin*ratio, currentXmax*ratio);
    m_renderer->set_y_axis_range(currentYmin*ratio, currentYmax*ratio);

    m_renderer->update();

    delete psf;
}
