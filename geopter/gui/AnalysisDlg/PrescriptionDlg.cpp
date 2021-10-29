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

