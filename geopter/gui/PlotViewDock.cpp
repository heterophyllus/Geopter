#include <iostream>

#include "PlotViewDock.h"

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

PlotViewDock::PlotViewDock(QString label, OpticalSystem* sys, QWidget *parent):
    ads::CDockWidget(label,parent),
    m_settingDlgPtr(nullptr),
    m_opticalSystem(sys)
{
    this->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    this->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    this->setMinimumSize(300,200);

    // QCustomPlot
    m_customPlot = new QCustomPlot;
    m_customPlot->setMinimumSize(0,0);
    this->setWidget(m_customPlot);

    // Tool Bar
    m_toolbar = new QToolBar(this);

    auto actionUpdate = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_BrowserReload ),"Update");
    auto actionSetting = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_FileDialogContentsView ),"Setting");
    auto actionSave = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_DialogSaveButton ),"Save");
    this->setToolBar(m_toolbar);

    QObject::connect(actionUpdate,  SIGNAL(triggered()), this, SLOT(updatePlot()));
    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
}

PlotViewDock::~PlotViewDock()
{
    m_settingDlgPtr.reset();

    try {
        delete m_customPlot;
    }  catch (...) {
        qDebug() << "delete error (QCustomPlot)";
    }

    delete m_toolbar;
}

QCustomPlot* PlotViewDock::customPlot()
{
    return m_customPlot;
}

void PlotViewDock::showSettingDlg()
{
    if(m_settingDlgPtr){
        if(m_settingDlgPtr->exec() == QDialog::Accepted){
            updateContent();
        }
    }
}

void PlotViewDock::saveToFile()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save to"),"",tr("PNG file(*.png);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    m_customPlot->savePng(filePath);
}

void PlotViewDock::updateContent()
{
    m_settingDlgPtr->updateParentDockContent();
}
