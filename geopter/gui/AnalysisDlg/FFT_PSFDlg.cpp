#include "FFT_PSFDlg.h"
#include "ui_FFT_PSFDlg.h"

FFT_PSFDlg::FFT_PSFDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
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
    int numField = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    for(int fi = 0; fi < numField; fi ++) {
        fieldComboItems << QString::number(fi);
    }
    ui->fieldCombo->addItems(fieldComboItems);
    ui->fieldCombo->setCurrentIndex(0);

    // wavelength
    QStringList wvlComboItems;
    int numWvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
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
    m_opticalSystem->UpdateModel();

    int fieldIndex = ui->fieldCombo->currentIndex();
    int wvlIndex = ui->wvlCombo->currentIndex();
    int ndim = 16 * pow(2, ui->samplingCombo->currentIndex());
    double L = ui->sideLengthEdit->text().toDouble();

    int type = ui->typeCombo->currentIndex();
    int colormap = ui->colormapCombo->currentIndex();


    Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fieldIndex);
    double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wvlIndex)->Value();

    DiffractivePSF* psf = new DiffractivePSF(m_opticalSystem);
    /*
    psf->from_opd_trace(m_opticalSystem, fld, wvl, ndim, L);
    */

    auto psf_grid = psf->Create(fld, wvl, ndim);


    m_renderer->Clear();
    m_renderer->DrawHist2d(psf_grid->ValueData(), type, colormap);

    m_renderer->SetXaxisRange(0, ndim);
    m_renderer->SetYaxisRange(0, ndim);

    m_renderer->Update();

    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    std::ostringstream oss;
    oss << psf_grid->ValueData().format(CleanFmt) << std::endl;
    m_parentDock->setText(oss);
    m_parentDock->setCurrentTab(1);

    delete psf;
}
