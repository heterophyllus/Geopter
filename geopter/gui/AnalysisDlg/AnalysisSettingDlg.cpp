#include "AnalysisSettingDlg.h"


AnalysisSettingDlg::AnalysisSettingDlg(OpticalSystem* sys, QWidget *parent) :
    QDialog(parent),
    m_opticalSystem(sys)
{

}

void AnalysisSettingDlg::updateParentDockContent()
{

}

int AnalysisSettingDlg::parentDockType() const
{
    return 0;
}
