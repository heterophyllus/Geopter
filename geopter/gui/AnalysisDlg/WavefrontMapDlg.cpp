#include "WavefrontMapDlg.h"
#include "ui_WavefrontMapDlg.h"

WavefrontMapDlg::WavefrontMapDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::WavefrontMapDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Wavefront Map Setting");

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    // sampling
    QStringList samplingComboItems;
    samplingComboItems << "16x16" << "32x32" << "64x64" << "128x128" << "256x256" << "512x512";
    ui->samplingCombo->clear();
    ui->samplingCombo->addItems(samplingComboItems);

    //field combo
    QStringList fieldComboItems;
    int fieldCount = FieldSpec::number_of_fields();
    for(int fi = 0; fi < fieldCount; fi++){
        fieldComboItems.append( QString::number(fi) );
    }
    ui->fieldCombo->clear();
    ui->fieldCombo->addItems(fieldComboItems);

    //wvl combo
    QStringList wvlComboItems;
    int wvlCount = m_opticalSystem->optical_spec()->spectral_region()->number_of_wavelengths();
    for(int wi = 0; wi < wvlCount; wi++){
        double wvl = m_opticalSystem->optical_spec()->spectral_region()->wavelength(wi)->value();
        wvlComboItems.append( "W" + QString::number(wi) + ": " +  QString::number(wvl));
    }
    ui->wavelengthCombo->clear();
    ui->wavelengthCombo->addItems(wvlComboItems);
}

WavefrontMapDlg::~WavefrontMapDlg()
{
    delete ui;
}

void WavefrontMapDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();
    int fieldIndex = ui->fieldCombo->currentIndex();
    int wvlIndex = ui->wavelengthCombo->currentIndex();
    int nrd_pow = ui->samplingCombo->currentIndex();
    int nrd = 16 * pow(2, nrd_pow);

    m_renderer->clear();
    m_renderer->set_x_axis_range(0, nrd);
    m_renderer->set_y_axis_range(0, nrd);

    WavefrontMap *wf = new WavefrontMap(m_opticalSystem);
    Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fieldIndex);
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wavelength(wvlIndex)->value();
    auto data_grid = wf->create(fld, wvl, nrd);

    m_renderer->draw_hist2d(data_grid->value_data(), 0, 1);

    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    std::ostringstream oss;
    oss << data_grid->value_data().format(CleanFmt) << std::endl;


    delete wf;

    m_renderer->update();

    m_parentDock->setText(oss);
    m_parentDock->setCurrentTab(1);
}
