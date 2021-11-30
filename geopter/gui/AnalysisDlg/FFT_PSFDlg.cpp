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
    samplingComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512" << "1024x1024";
    ui->samplingCombo->clear();
    ui->samplingCombo->addItems(samplingComboItems);
    ui->samplingCombo->setCurrentIndex(3);

    // image plane side length
    ui->sideLengthEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->sideLengthEdit->setText("0.1");

    // type
    QStringList typeComboItems;
    typeComboItems << "Linear" << "Logarithmic";
    ui->typeCombo->clear();
    ui->typeCombo->addItems(typeComboItems);
    ui->typeCombo->setCurrentIndex(0);

    // colormap
    QStringList colormapComboItems;
    colormapComboItems << "Gray" << "Jet";
    ui->colormapCombo->clear();
    ui->colormapCombo->addItems(colormapComboItems);
    ui->colormapCombo->setCurrentIndex(1);

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
    double L = ui->sideLengthEdit->text().toDouble();

    int type = ui->typeCombo->currentIndex();
    int colormap = ui->colormapCombo->currentIndex();


    Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fieldIndex);
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wvlIndex)->value();

    DiffractivePSF* psf = new DiffractivePSF(m_opticalSystem);
    psf->from_opd_trace(m_opticalSystem, fld, wvl, ndim, L);


    m_renderer->clear();
    m_renderer->draw_hist2d(psf->to_matrix(), type, colormap);

    m_renderer->set_x_axis_range(0, ndim);
    m_renderer->set_y_axis_range(0, ndim);

    m_renderer->update();

    delete psf;
}
