#include "Analysis/first_order_data_dock.h"

FirstOrderDataDock::FirstOrderDataDock(QString label, OpticalSystem* sys, QWidget *parent):
    TextViewDock(label, parent),
    m_opticalSystem(sys)
{
    // This dock does not have setting dialog
    m_settingDlgPtr = nullptr;
}

FirstOrderDataDock::~FirstOrderDataDock()
{
    //m_opticalSystem = nullptr;
}

void FirstOrderDataDock::updateText()
{
    m_opticalSystem->update_model();

    clear();
    std::ostringstream oss;
    m_opticalSystem->first_order_data().print(oss);
    setStringStreamToText(oss);
}
