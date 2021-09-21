#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>

#include "main_window.h"
#include "./ui_main_window.h"

#include "text_view_dock.h"
#include "plot_view_dock.h"
#include "general_configuration_dialog.h"
#include "layout_dialog.h"
#include "paraxial_trace_dialog.h"
#include "single_ray_trace_dialog.h"
#include "transverse_ray_fan_dialog.h"
#include "longitudinal_setting_dialog.h"
#include "renderer_qcp.h"

#include "qdebugstream.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QTextEdit>
#include <QDebug>

using namespace ads;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Geopter");

    // File menu
    QObject::connect(ui->actionNew,  SIGNAL(triggered()), this, SLOT(newFile()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveAs()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(ui->actionPreference, SIGNAL(triggered()), this, SLOT(showPreference()));

    // Analysis menu
    QObject::connect(ui->actionPrescription,           SIGNAL(triggered()), this, SLOT(showPrescription()));
    QObject::connect(ui->action2DLayout,               SIGNAL(triggered()), this, SLOT(showLayout()));
    QObject::connect(ui->actionFirstOrderData,         SIGNAL(triggered()), this, SLOT(showFirstOrderData()));
    QObject::connect(ui->actionSingleRayTrace,         SIGNAL(triggered()), this, SLOT(showSingleRayTrace()));
    QObject::connect(ui->actionParaxialRayTrace,       SIGNAL(triggered()), this, SLOT(showParaxialRayTrace()));
    QObject::connect(ui->actionRayAberration ,         SIGNAL(triggered()), this, SLOT(showTransverseRayFan()));
    QObject::connect(ui->actionLongitudinalAberration, SIGNAL(triggered()), this, SLOT(showLongitudinal()));

    // Help menu
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));


    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);


    // create optical system
    opt_sys_ = std::make_shared<OpticalSystem>();

    agf_dir_path_ = QApplication::applicationDirPath().toStdString() + "/AGF";
    loadAgfsFromDir(QString().fromStdString(agf_dir_path_));

    opt_sys_->create_minimum_system();


    // set system editor as central dock
    m_dockManager = new CDockManager(this);
    m_systemEditorDock = new SystemEditorDock(opt_sys_, "System Editor");
    auto* CentralDockArea = m_dockManager->setCentralWidget(m_systemEditorDock );
    CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    m_systemEditorDock->syncUiWithSystem();

}

MainWindow::~MainWindow()
{
    opt_sys_.reset();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    m_dockManager->deleteLater();
    QMainWindow::closeEvent(event);
}


void MainWindow::loadAgfsFromDir(QString agfDir)
{
    std::vector< std::string > agf_paths;

    QStringList nameFilters;
    nameFilters.append("*.agf");
    nameFilters.append("*.AGF");

    QDir dir(agfDir);
    QStringList entry = dir.entryList(nameFilters, QDir::Files);
    for (QString file : entry) {
        qDebug() << dir.filePath(file);
        agf_paths.push_back(dir.filePath(file).toStdString());
    }

    bool ret = opt_sys_->material_lib()->load_agf_files(agf_paths);
    if(!ret){
        QMessageBox::warning(this,tr("Error") ,tr("AGF load error"));
    }

}


/*********************************************************************************************************************************
 *
 * File menu
 *
 * ********************************************************************************************************************************/
void MainWindow::newFile()
{
    opt_sys_.reset();
    opt_sys_ = std::make_shared<OpticalSystem>();

    opt_sys_->create_minimum_system();
    opt_sys_->update_model();

    m_systemEditorDock->setOpticalSystem(opt_sys_);
    m_systemEditorDock->syncUiWithSystem();

    QMessageBox::information(this,tr("Info"), tr("Created new lens"));
}

void MainWindow::saveAs()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this, tr("select JSON"), QApplication::applicationDirPath(), tr("JSON files(*.json);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    std::string json_path = filePath.toStdString();
    FileIO::save_to_json(*opt_sys_, json_path);

    QMessageBox::information(this,tr("Info"), tr("Saved to JSON file"));
}

void MainWindow::openFile()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("Open JSON"),
                                                          QApplication::applicationDirPath(),
                                                          tr("JSON files(*.json)"));
    if(filePaths.empty()){
        //QMessageBox::warning(this,tr("Canceled"), tr("Canceled"));
        return;
    }

    std::string json_path = filePaths.first().toStdString();
    FileIO::load_from_json(*opt_sys_,json_path);

    opt_sys_->update_model();

    m_systemEditorDock->setOpticalSystem(opt_sys_);
    m_systemEditorDock->syncUiWithSystem();

    QMessageBox::information(this,tr("Info"), tr("OpticalSystem newly loaded"));
}

void MainWindow::showPreference()
{
    GeneralConfigurationDialog* dlg = new GeneralConfigurationDialog(opt_sys_, this);
    dlg->exec();
    delete dlg;
}

/*********************************************************************************************************************************
 *
 * Analysis menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showPrescription()
{
    TextViewDock *dock = new TextViewDock("Prescription");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    std::ostringstream oss;
    opt_sys_->print(oss);

    // This analysis does not have setting dialog so just shows the result.
    dock->setStringStreamToText(oss);
}


void MainWindow::showLayout()
{
    PlotViewDock *dock = new PlotViewDock("Layout");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<LayoutDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showFirstOrderData()
{
    std::ostringstream ss;

    opt_sys_->update_model();
    opt_sys_->first_order_data().print(ss);

    TextViewDock *dock = new TextViewDock("First Order Data");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    // This analysis does not have setting dialog so just shows the result.
    dock->setStringStreamToText(ss);
}


void MainWindow::showParaxialRayTrace()
{
    opt_sys_->update_model();

    TextViewDock *dock = new TextViewDock("Paraxial Ray Trace");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<ParaxialTraceDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();

}

void MainWindow::showSingleRayTrace()
{
    TextViewDock *dock = new TextViewDock("Single Ray Trace");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<SingleRayTraceDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showTransverseRayFan()
{
    PlotViewDock *dock = new PlotViewDock("Transverse Ray Fan");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<TransverseRayFanDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();   
}

void MainWindow::showLongitudinal()
{
    PlotViewDock *dock = new PlotViewDock("Longitudinal Aberration");
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<LongitudinalSettingDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}



/*********************************************************************************************************************************
 *
 * Tool menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showDebugStream()
{

}



/*********************************************************************************************************************************
 *
 * Help menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showAbout()
{
    // TODO: Dialog to show description and licence notice should be implemented

    QMessageBox::information(this,tr("About"), tr("Geopter v0.1.0"));
}

