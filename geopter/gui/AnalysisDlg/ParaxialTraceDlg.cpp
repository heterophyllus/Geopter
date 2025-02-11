#include <iostream>
#include <sstream>
#include <iomanip>

#include "ParaxialTraceDlg.h"
#include "ui_ParaxialTraceDlg.h"


ParaxialTraceDlg::ParaxialTraceDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::ParaxialTraceDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Paraxial Ray Trace Setting");

    //wvl combo
    const int num_wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(i)->Value());
        ui->comboWvl->addItem(wvl_item);
    }

    int refWvlIndex = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->ReferenceIndex();
    ui->comboWvl->setCurrentIndex(refWvlIndex);

}

ParaxialTraceDlg::~ParaxialTraceDlg()
{
    delete ui;
}

void ParaxialTraceDlg::updateParentDockContent()
{
    int wi = ui->comboWvl->currentIndex();
    double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();

    std::ostringstream oss;

    oss << "Paraxial Trace" << std::endl;
    oss << std::endl;
    oss << "Wavelength: " << wvl << std::endl;
    oss << std::endl;

    ParaxialTrace* tracer = new ParaxialTrace(m_opticalSystem);
    auto ax_ray = tracer->TraceParaxialAxisRay(wvl);

    oss << "Axial Ray..." << std::endl;
    ax_ray->Print(oss);
    oss << std::endl;

    auto pr_ray = tracer->TraceParaxialChiefRay(wvl);
    oss << "Principle Ray..." << std::endl;
    pr_ray->Print(oss);
    oss << std::endl;

    m_parentDock->setText(oss);
}
