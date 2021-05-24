#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "optical_core.h"
using namespace geopter;

class LensDataManagerDock;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    // File menu
    void newFile();
    void saveAs();
    void openFile();
    void showConfig();

    // Analysis menu
    void showLayout();
    void showFirstOrderData();
    void showRealRayTrace();
    void showParaxialRayTrace();
    void showRayFan();
    void showLongitudinal();

    // Help menu
    void showAbout();

private:
    Ui::MainWindow *ui;

    ads::CDockManager* dockManager_;
    LensDataManagerDock* lens_data_manager_;

    std::shared_ptr<OpticalModel> opt_model_;
    std::shared_ptr<Database> database_;
    std::string agf_dir_path_;

};
#endif // MAINWINDOW_H
