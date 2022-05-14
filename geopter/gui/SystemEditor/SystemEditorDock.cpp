#include <QApplication>
#include <QVBoxLayout>

#include "SystemEditorDock.h"
#include "SpecEditor/SpecEditorDlg.h"

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
    m_systemEditorWidget->setOpticalSystem(m_opticalSystem);
}

SystemEditorWidget* SystemEditorDock::systemEditorWidget()
{
    return m_systemEditorWidget;
}

void SystemEditorDock::rebuildUi()
{
    try{
        delete m_systemEditorWidget;
    }catch(...){
        qDebug() << "delete error : system editor widget";
    }

    m_systemEditorWidget = new SystemEditorWidget(m_opticalSystem, this);
    this->setWidget(m_systemEditorWidget);
}
