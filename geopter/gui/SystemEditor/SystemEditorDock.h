#ifndef SYSTEMEDITORDOCK_H
#define SYSTEMEDITORDOCK_H

#include <QToolBar>
#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include "SystemEditorWidget.h"


/** Main widget to edit OpticalSystem. This dock is used as a central dock of MainWindow */
class SystemEditorDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit SystemEditorDock(std::shared_ptr<OpticalSystem> opt_sys, QString label, QWidget *parent = nullptr);
    ~SystemEditorDock();

    void setOpticalSystem(std::shared_ptr<OpticalSystem> sys);
    SystemEditorWidget* systemEditorWidget();
    void rebuildUi();

private :
    std::shared_ptr<OpticalSystem> m_opticalSystem;
    SystemEditorWidget* m_systemEditorWidget;
};

#endif // SYSTEMEDITORDOCK_H
