#include "prescription_dock.h"

PrescriptionDock::PrescriptionDock(QString label, OpticalSystem* sys, QWidget *parent) :
    TextViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = nullptr;
}

PrescriptionDock::~PrescriptionDock()
{
    //m_opticalSystem = nullptr;
}

void PrescriptionDock::updateText()
{
    m_opticalSystem->update_model();

    std::ostringstream oss;
    m_opticalSystem->print(oss);
    setStringStreamToText(oss);
}
