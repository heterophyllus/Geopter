#include <iostream>
#include <sstream>
#include <iomanip>

#include "prescription_setting_dialog.h"
#include "ui_prescription_setting_dialog.h"

PrescriptionSettingDialog::PrescriptionSettingDialog(OpticalSystem* sys, TextViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::PrescriptionSettingDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    setWindowTitle("Prescription Setting");

    //default
    ui->generalInfoCheck->setCheckState(Qt::Checked);
    ui->specCheck->setCheckState(Qt::Checked);
    ui->surfaceDataCheck->setCheckState(Qt::Checked);
}

PrescriptionSettingDialog::~PrescriptionSettingDialog()
{
    delete ui;
}

void PrescriptionSettingDialog::updateParentDockContent()
{
    bool doGeneralInfo = ui->generalInfoCheck->checkState();
    bool doSpec = ui->specCheck->checkState();
    bool doSurfaceData = ui->surfaceDataCheck->checkState();
    bool doFirstOrderData = ui->firstOrderDataCheck->checkState();

    m_opticalSystem->update_model();

    std::ostringstream oss;

    oss << "Prescription..." << std::endl;
    oss << std::endl;

    if(doGeneralInfo) {
        oss << "Title: " << m_opticalSystem->title() << std::endl;
        oss << std::endl;
        oss << "Note: " << std::endl;
        oss << m_opticalSystem->note() << std::endl;
    }

    if(doSpec) {
        //oss << "Optical Specs..." << std::endl;
        //oss << std::endl;
        m_opticalSystem->optical_spec()->print(oss);
        oss << std::endl;
    }

    if(doSurfaceData) {
        oss << "Surface Data..." << std::endl;
        m_opticalSystem->optical_assembly()->print(oss);
        oss << std::endl;
    }

    if(doFirstOrderData) {
        oss << "First Order Data..." << std::endl;
        m_opticalSystem->first_order_data().print(oss);
        oss << std::endl;
    }

    m_parentDock->setStringStreamToText(oss);

}

