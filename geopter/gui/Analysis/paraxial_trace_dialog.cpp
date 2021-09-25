#include <iostream>
#include <sstream>
#include <iomanip>

#include "paraxial_trace_dialog.h"
#include "ui_paraxial_trace_dialog.h"


ParaxialTraceDialog::ParaxialTraceDialog(OpticalSystem* sys, TextViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::ParaxialTraceDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
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

ParaxialTraceDialog::~ParaxialTraceDialog()
{
    delete ui;
}

void ParaxialTraceDialog::updateParentDockContent()
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
