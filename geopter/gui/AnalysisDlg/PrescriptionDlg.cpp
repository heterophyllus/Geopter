#include <iostream>
#include <sstream>
#include <iomanip>

#include "PrescriptionDlg.h"
#include "ui_prescriptiondlg.h"

PrescriptionDlg::PrescriptionDlg(OpticalSystem* sys, TextViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::PrescriptionDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    setWindowTitle("Prescription Setting");

    //default
    ui->generalInfoCheck->setCheckState(Qt::Checked);
    ui->specCheck->setCheckState(Qt::Checked);
    ui->firstOrderDataCheck->setCheckState(Qt::Checked);
    ui->lensDataCheck->setCheckState(Qt::Checked);
    ui->surfaceDataCheck->setCheckState(Qt::Checked);
}

PrescriptionDlg::~PrescriptionDlg()
{
    delete ui;
}

void PrescriptionDlg::updateParentDockContent()
{
    bool doGeneralInfo = ui->generalInfoCheck->checkState();
    bool doSpec = ui->specCheck->checkState();
    bool doLensData = ui->lensDataCheck->checkState();
    bool doFirstOrderData = ui->firstOrderDataCheck->checkState();
    bool doSurfaceData = ui->surfaceDataCheck->checkState();

    m_opticalSystem->update_model();

    std::ostringstream oss;

    oss << "PRESCRIPTION..." << std::endl;
    oss << std::endl;

    if(doGeneralInfo) {
        oss << "TITLE: " << m_opticalSystem->title() << std::endl;
        oss << std::endl;
        oss << "NOTE: " << std::endl;
        oss << m_opticalSystem->note() << std::endl;
        oss << std::endl;
    }

    if(doSpec) {
        m_opticalSystem->optical_spec()->print(oss);
        oss << std::endl;
    }

    if(doFirstOrderData) {
        oss << "FIRST ORDER DATA..." << std::endl;
        m_opticalSystem->paraxial_data()->print(oss);
        oss << std::endl;
    }

    if(doLensData) {
        oss << "LENS DATA..." << std::endl;
        m_opticalSystem->optical_assembly()->print(oss);
        oss << std::endl;
    }

    if(doSurfaceData){
        oss << "SURFACE DATA..." << std::endl;
        const int num_srf = m_opticalSystem->optical_assembly()->surface_count();
        for(int si = 0; si < num_srf; si++){
            Surface* srf = m_opticalSystem->optical_assembly()->surface(si);
            if(srf->profile()->name() == "SPH"){
                continue;
            }else{
                oss << "Surface " << si << std::endl;
                srf->profile()->print(oss);
                oss << std::endl;
            }
        }
    }


    m_parentDock->setStringStreamToText(oss);

}

