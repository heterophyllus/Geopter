#include <iostream>

#include "plot_view_dock.h"
#include "qcustomplot.h"

#include <QDialog>
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

    customPlot_ = new QCustomPlot;
    this->setWidget(customPlot_);
    customPlot_->setMinimumSize(0,0);

    // QCustomPlot
    customPlot_->setInteraction(QCP::iRangeDrag, true);
    customPlot_->setInteraction(QCP::iRangeZoom, true);
    customPlot_->setInteraction(QCP::iSelectItems, true);

    // Tool Bar
    toolbar_ = new QToolBar(this);
    auto actionSetting = toolbar_->addAction("Setting");
    auto actionSave = toolbar_->addAction("Save");
    this->setToolBar(toolbar_);

    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
}

PlotViewDock::~PlotViewDock()
{
    settingDlgPtr_.reset();


    try {
        delete customPlot_;
    }  catch (...) {
        std::cout << "Error: ~PlotViewDock" << std::endl;
    }


    //delete toolbar_;
}

void PlotViewDock::possessDlg(std::unique_ptr<QDialog> dlg)
{
    settingDlgPtr_ = std::move(dlg);
}

QCustomPlot* PlotViewDock::customPlot()
{
    return customPlot_;
}

void PlotViewDock::showSettingDlg()
{
    if(settingDlgPtr_){
        settingDlgPtr_->exec();
    }
}

void PlotViewDock::saveToFile()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save to"),"",tr("PNG file(*.png);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    customPlot_->savePng(filePath);
}
