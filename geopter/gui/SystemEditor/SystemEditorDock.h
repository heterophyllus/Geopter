#ifndef SYSTEMEDITORDOCK_H
#define SYSTEMEDITORDOCK_H

#include <QToolBar>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "SystemEditorWidget.h"

#include "optical.h"
using namespace geopter;

/** Main widget to edit OpticalSystem. This dock is used as a central dock of MainWindow */
class SystemEditorDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit SystemEditorDock(std::shared_ptr<OpticalSystem> opt_sys, QString label, QWidget *parent = nullptr);
    ~SystemEditorDock();

public:
    void setOpticalSystem(std::shared_ptr<OpticalSystem> sys);
    void syncUiWithSystem();
    void syncSystemWithUi();

    SystemEditorWidget* systemEditorWidget();

public slots:
    void updateOpticalSystem();

private :
    std::shared_ptr<OpticalSystem> m_opticalSystem;
    SystemEditorWidget *m_systemEditorWidget;
    QToolBar *m_toolBar;
};

#endif // SYSTEMEDITORDOCK_H
