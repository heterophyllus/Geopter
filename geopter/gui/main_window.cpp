#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>


#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QTextEdit>
#include <QDebug>

#include "main_window.h"
#include "./ui_main_window.h"

#include "text_view_dock.h"
#include "plot_view_dock.h"
#include "general_configuration_dialog.h"

#include "renderer_qcp.h"

#include "analysisdialogs/analysis_setting_dialog.h"
#include "analysisdialogs/layout_dialog.h"
#include "analysisdialogs/transverse_ray_fan_dialog.h"
#include "analysisdialogs/single_ray_trace_dialog.h"
#include "analysisdialogs/longitudinal_setting_dialog.h"
#include "analysisdialogs/paraxial_trace_dialog.h"
#include "analysisdialogs/prescription_setting_dialog.h"
#include "analysisdialogs/field_curvature_setting_dialog.h"
#include "analysisdialogs/chromatic_focusshift_setting_dialog.h"
#include "analysisdialogs/spot_diagram_setting_dialog.h"

#include "qdebugstream.h"

#include "gui/PythonQtScriptingConsole.h"

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

    // Edit menu
    QObject::connect(ui->actionSetVigFactors, SIGNAL(triggered()), this, SLOT(setVignettingFactors()));

    // Analysis menu
    QObject::connect(ui->actionPrescription,           SIGNAL(triggered()), this, SLOT(showPrescription()));
    QObject::connect(ui->action2DLayout,               SIGNAL(triggered()), this, SLOT(showLayout()));
    QObject::connect(ui->actionSingleRayTrace,         SIGNAL(triggered()), this, SLOT(showSingleRayTrace()));
    QObject::connect(ui->actionParaxialRayTrace,       SIGNAL(triggered()), this, SLOT(showParaxialRayTrace()));
    QObject::connect(ui->actionRayAberration ,         SIGNAL(triggered()), this, SLOT(showTransverseRayFan()));
    QObject::connect(ui->actionLongitudinalAberration, SIGNAL(triggered()), this, SLOT(showLongitudinal()));
    QObject::connect(ui->actionFieldCurvature,         SIGNAL(triggered()), this, SLOT(showFieldCurvature()));
    QObject::connect(ui->actionChromaticFocusShift,    SIGNAL(triggered()), this, SLOT(showChromaticFocusShift()));
    QObject::connect(ui->actionSpotDiagram,            SIGNAL(triggered()), this, SLOT(showSpotDiagram()));

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


    // create python console
    PythonQtScriptingConsole* console = new PythonQtScriptingConsole(NULL, PythonQt::self()->getMainModule());
    CDockWidget* ConsoleDock = new CDockWidget("Console");
    ConsoleDock->setWidget(console);
    ConsoleDock->setFeature(CDockWidget::DockWidgetClosable, false);
    m_dockManager->addDockWidget(DockWidgetArea::BottomDockWidgetArea, ConsoleDock);

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
    opt_sys_->create_minimum_system();
    //opt_sys_->update_model();

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
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Open JSON"), QApplication::applicationDirPath(),tr("JSON files(*.json)"));
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
 * Edit menu
 *
 * ********************************************************************************************************************************/
void MainWindow::setVignettingFactors()
{
    opt_sys_->update_vignetting_factors();
    opt_sys_->update_model();
    m_systemEditorDock->syncUiWithSystem();
}


/*********************************************************************************************************************************
 *
 * Analysis menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showPrescription()
{
    TextViewDock *dock = new TextViewDock("Prescription", opt_sys_.get());
    dock->createSettingDialog<PrescriptionSettingDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updateText();
}


void MainWindow::showLayout()
{
    PlotViewDock *dock = new PlotViewDock("2D layout", opt_sys_.get());
    dock->createSettingDialog<LayoutDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updatePlot();
}


void MainWindow::showParaxialRayTrace()
{
    TextViewDock *dock = new TextViewDock("Paraxial Ray Trace", opt_sys_.get());
    dock->createSettingDialog<ParaxialTraceDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updateText();
}

void MainWindow::showSingleRayTrace()
{
    TextViewDock *dock = new TextViewDock("Single Ray Trace", opt_sys_.get());
    dock->createSettingDialog<SingleRayTraceDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updateText();
}

void MainWindow::showSpotDiagram()
{
    PlotViewDock *dock = new PlotViewDock("Spot Diagram", opt_sys_.get());
    dock->createSettingDialog<SpotDiagramSettingDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    //dock->resize(300,200);
    dock->updatePlot();
}

void MainWindow::showTransverseRayFan()
{
    PlotViewDock *dock = new PlotViewDock("Transverse Aberration", opt_sys_.get());
    dock->createSettingDialog<TransverseRayFanDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updatePlot();
}

void MainWindow::showLongitudinal()
{
    PlotViewDock *dock = new PlotViewDock("Longitudinal Aberration", opt_sys_.get());
    dock->createSettingDialog<LongitudinalSettingDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updatePlot();
}

void MainWindow::showFieldCurvature()
{
    PlotViewDock *dock = new PlotViewDock("Field Curvature", opt_sys_.get());
    dock->createSettingDialog<FieldCurvatureSettingDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updatePlot();
}

void MainWindow::showChromaticFocusShift()
{
    PlotViewDock *dock = new PlotViewDock("Chromatic Focus Shift", opt_sys_.get());
    dock->createSettingDialog<ChromaticFocusShiftSettingDialog>();
    m_dockManager->addDockWidgetFloating(dock);
    dock->resize(300,200);
    dock->updatePlot();
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

