#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>

#include "main_window.h"
#include "./ui_main_window.h"

#include "lens_data_manager_dock.h"
#include "text_view_dock.h"
#include "plot_view_dock.h"
#include "general_configuration_dialog.h"
#include "layout_dialog.h"
#include "real_ray_trace_dialog.h"
#include "transverse_ray_fan_dialog.h"
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
    QObject::connect(ui->actionConfig, SIGNAL(triggered()), this, SLOT(showConfig()));

    // Analysis menu
    QObject::connect(ui->actionLayout,             SIGNAL(triggered()), this, SLOT(showLayout()));
    QObject::connect(ui->actionFirst_Order_Data,   SIGNAL(triggered()), this, SLOT(showFirstOrderData()));
    QObject::connect(ui->actionReal_Ray_Trace,     SIGNAL(triggered()), this, SLOT(showRealRayTrace()));
    QObject::connect(ui->actionParaxial_Ray_Trace, SIGNAL(triggered()), this, SLOT(showParaxialRayTrace()));
    QObject::connect(ui->actionTransverse_Ray_Fan, SIGNAL(triggered()), this, SLOT(showRayFan()));

    // Help menu
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    // Lens Data Manager
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);



    // try loading AGF
    // database(glass catalog .etc)
    database_ = std::make_shared<Database>();
    agf_dir_path_ = QApplication::applicationDirPath().toStdString() + "/AGF";
    if(!database_->load_all_agf(agf_dir_path_)){
        QMessageBox::warning(this, tr("AGF load error"), tr("Failed to load AGF from the directory") + QString().fromStdString(agf_dir_path_));
    }

    // optical model
    opt_model_ = std::make_shared<OpticalModel>(database_);
    opt_model_->set_default_model();

    // set lens data manager as central dock
    dockManager_ = new CDockManager(this);
    lens_data_manager_ = new LensDataManagerDock(opt_model_, "Lens Data");
    lens_data_manager_->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    auto* CentralDockArea = dockManager_->setCentralWidget(lens_data_manager_);
    CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    lens_data_manager_->initialize();
    lens_data_manager_->setOpticalModel(opt_model_);
    lens_data_manager_->syncTableWithModel();
}

MainWindow::~MainWindow()
{
    opt_model_.reset();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    dockManager_->deleteLater();
    QMainWindow::closeEvent(event);
}


/*********************************************************************************************************************************
 *
 * File menu
 *
 * ********************************************************************************************************************************/
void MainWindow::newFile()
{
    opt_model_.reset();
    opt_model_ = std::make_shared<OpticalModel>(database_);

    opt_model_->set_default_model();
    opt_model_->update_model();

    lens_data_manager_->setOpticalModel(opt_model_);
    lens_data_manager_->syncTableWithModel();

    QMessageBox::information(this,tr("Info"), tr("Created new lens"));
}

void MainWindow::saveAs()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this,
                                                          tr("select JSON"),
                                                          QApplication::applicationDirPath(),
                                                          tr("JSON files(*.json);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    std::string json_path = filePath.toStdString();

    FileIO::save_to_json(*opt_model_, json_path);

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
    FileIO::load_from_json(*opt_model_,json_path);

    opt_model_->update_model();
    opt_model_->update_model(); // FIXME: must be twice updated

    lens_data_manager_->setOpticalModel(opt_model_);
    lens_data_manager_->syncTableWithModel();

    QMessageBox::information(this,tr("Info"), tr("OpticalModel newly loaded"));
}

void MainWindow::showConfig()
{
    GeneralConfigurationDialog* dlg = new GeneralConfigurationDialog(opt_model_,this);
    dlg->exec();
    delete dlg;
}

/*********************************************************************************************************************************
 *
 * Analysis menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showLayout()
{
    PlotViewDock *dock = new PlotViewDock("Layout");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<LayoutDialog>(opt_model_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showFirstOrderData()
{
    std::ostringstream ss;

    opt_model_->paraxial_model()->first_order_data().print(ss);

    TextViewDock *dock = new TextViewDock("First Order Data");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    // This analysis does not have setting dialog so just shows the result.
    dock->setStringStreamToText(ss);
}


void MainWindow::showParaxialRayTrace()
{
    opt_model_->paraxial_model()->update_model();
    ParaxialRay ax_ray = opt_model_->paraxial_model()->ax();
    ParaxialRay pr_ray = opt_model_->paraxial_model()->pr();

    std::ostringstream oss;

    oss << "Axial Merginal Ray : " << std::endl;
    ax_ray.print(oss);

    oss << "Principle Ray : " << std::endl;
    pr_ray.print(oss);


    std::cout << oss.str() << std::endl;

    TextViewDock *dock = new TextViewDock("Paraxial Ray");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->setStringStreamToText(oss);

}

void MainWindow::showRealRayTrace()
{
    TextViewDock *dock = new TextViewDock("Real Ray Trace");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<RealRayTraceDialog>(opt_model_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showRayFan()
{
    PlotViewDock *dock = new PlotViewDock("Transverse Ray Fan");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<TransverseRayFanDialog>(opt_model_.get(), dock));
    dock->showSettingDlg();   
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

