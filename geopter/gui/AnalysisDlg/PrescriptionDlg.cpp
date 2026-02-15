#include <iostream>
#include <sstream>
#include <iomanip>

#include "PrescriptionDlg.h"
#include "ui_PrescriptionDlg.h"

PrescriptionDlg::PrescriptionDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
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

    m_opticalSystem->UpdateModel();

    std::ostringstream oss;

    oss << "PRESCRIPTION..." << std::endl;
    oss << std::endl;

    if(doGeneralInfo) {
        oss << "TITLE: " << m_opticalSystem->Title() << std::endl;
        oss << std::endl;
        oss << "NOTE: " << std::endl;
        oss << m_opticalSystem->Note() << std::endl;
        oss << std::endl;
    }

    if(doSpec) {
        m_opticalSystem->GetOpticalSpec()->print(oss);
        oss << std::endl;
    }

    if(doFirstOrderData) {
        oss << "FIRST ORDER DATA..." << std::endl;
        m_opticalSystem->GetFirstOrderData()->Print(oss);
        oss << std::endl;
    }

    if(doLensData) {
        oss << "LENS DATA..." << std::endl;
        m_opticalSystem->GetOpticalAssembly()->Print(oss);
        oss << std::endl;
    }

    if(doSurfaceData){
        oss << "SURFACE DATA..." << std::endl;
        const int num_srf = m_opticalSystem->GetOpticalAssembly()->NumberOfSurfaces();
        for(int si = 0; si < num_srf; si++){
            Surface* srf = m_opticalSystem->GetOpticalAssembly()->GetSurface(si);
            if(srf->IsProfile<Spherical>()){
                continue;
            }else if(srf->IsProfile<EvenPolynomial>()){
                oss << "Surface " << si << std::endl;
                srf->Profile<EvenPolynomial>()->Print(oss);
                oss << std::endl;
            }else if(srf->IsProfile<OddPolynomial>()){
                oss << "Surface " << si << std::endl;
                srf->Profile<OddPolynomial>()->Print(oss);
                oss << std::endl;
            }else{
                qDebug() << "Unknown Surface Profile";
            }
        }
    }


    m_parentDock->setText(oss);

}

