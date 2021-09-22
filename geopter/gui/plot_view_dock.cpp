#include <iostream>

#include "plot_view_dock.h"
#include "qcustomplot.h"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

PlotViewDock::PlotViewDock(QString label, QWidget *parent):
    ads::CDockWidget(label,parent)
{
    this->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    this->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    //this->resize(300,200);
    this->setMinimumSize(300,200);


    // QCustomPlot
    m_customPlot = new QCustomPlot;
    this->setWidget(m_customPlot);
    m_customPlot->setMinimumSize(0,0);

    m_renderer = new RendererQCP(m_customPlot);

    // Tool Bar
    m_toolbar = new QToolBar(this);

    auto actionUpdate = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_BrowserReload ),"Update");
    auto actionSetting = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_FileDialogContentsView ),"Setting");
    auto actionSave = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_DialogSaveButton ),"Save");
    this->setToolBar(m_toolbar);

    QObject::connect(actionUpdate, SIGNAL(triggered()), this, SLOT(updatePlot()));
    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
}

PlotViewDock::~PlotViewDock()
{
    m_settingDlgPtr.reset();

    delete m_renderer;

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
            updatePlot();
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

void PlotViewDock::updatePlot()
{

}
