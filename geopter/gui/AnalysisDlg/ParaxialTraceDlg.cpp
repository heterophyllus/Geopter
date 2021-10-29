#include <iostream>
#include <sstream>
#include <iomanip>

#include "ParaxialTraceDlg.h"
#include "ui_paraxialtracedlg.h"


ParaxialTraceDlg::ParaxialTraceDlg(OpticalSystem* sys, TextViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::ParaxialTraceDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Paraxial Ray Trace Setting");

    //wvl combo
    const int num_wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(m_opticalSystem->optical_spec()->spectral_region()->wvl(i)->value());
        ui->comboWvl->addItem(wvl_item);
    }

    int refWvlIndex = m_opticalSystem->optical_spec()->spectral_region()->reference_index();
    ui->comboWvl->setCurrentIndex(refWvlIndex);

}

ParaxialTraceDlg::~ParaxialTraceDlg()
{
    delete ui;
}

void ParaxialTraceDlg::updateParentDockContent()
{
    int wi = ui->comboWvl->currentIndex();
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    std::ostringstream oss;

    oss << "Paraxial Trace" << std::endl;
    oss << std::endl;
    oss << "Wavelength: " << wvl << std::endl;
    oss << std::endl;

    oss << "Axial Ray..." << std::endl;
    m_opticalSystem->axial_ray(wi)->print(oss);
    oss << std::endl;

    oss << "Principle Ray..." << std::endl;
    m_opticalSystem->principle_ray(wi)->print(oss);
    oss << std::endl;

    m_parentDock->setStringStreamToText(oss);
}
