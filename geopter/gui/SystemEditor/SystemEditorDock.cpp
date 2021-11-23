#include <QApplication>
#include <QVBoxLayout>

#include "SystemEditorDock.h"
#include "SpecEditorDlg.h"

SystemEditorDock::SystemEditorDock(std::shared_ptr<OpticalSystem> opt_sys, QString label, QWidget *parent) :
    ads::CDockWidget(label, parent),
    m_opticalSystem(opt_sys)
{
    m_systemEditorWidget = new SystemEditorWidget(m_opticalSystem, this);

    this->setWidget(m_systemEditorWidget);
}

SystemEditorDock::~SystemEditorDock()
{
    delete m_systemEditorWidget;
    m_opticalSystem.reset();
}


void SystemEditorDock::setOpticalSystem(std::shared_ptr<OpticalSystem> sys)
{
    m_opticalSystem = sys;
}


void SystemEditorDock::updateUi()
{
    m_systemEditorWidget->lensSpreadSheet()->reload();
}
