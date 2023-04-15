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
    int fieldCount = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    for(int fi = 0; fi < fieldCount; fi++){
        fieldComboItems.append( QString::number(fi) );
    }
    ui->fieldCombo->clear();
    ui->fieldCombo->addItems(fieldComboItems);

    //wvl combo
    QStringList wvlComboItems;
    int wvlCount = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    for(int wi = 0; wi < wvlCount; wi++){
        double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();
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
    m_opticalSystem->UpdateModel();
    int fieldIndex = ui->fieldCombo->currentIndex();
    int wvlIndex = ui->wavelengthCombo->currentIndex();
    int nrd_pow = ui->samplingCombo->currentIndex();
    int nrd = 16 * pow(2, nrd_pow);

    m_renderer->Clear();
    m_renderer->SetXaxisRange(0, nrd);
    m_renderer->SetYaxisRange(0, nrd);

    WavefrontMap *wf = new WavefrontMap(m_opticalSystem);
    Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fieldIndex);
    double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wvlIndex)->Value();
    auto data_grid = wf->Create(fld, wvl, nrd);

    m_renderer->DrawHist2d(data_grid->ValueData(), 0, 1);

    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    std::ostringstream oss;
    oss << data_grid->ValueData().format(CleanFmt) << std::endl;


    delete wf;

    m_renderer->Update();

    m_parentDock->setText(oss);
    m_parentDock->setCurrentTab(1);
}
