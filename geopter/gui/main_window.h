#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "optical.h"
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
    void showReport();
    void showLayout();
    void showFirstOrderData();
    void showSingleRayTrace();
    void showParaxialRayTrace();
    void showRayFan();
    void showLongitudinal();

    // Tool menu
    void showDebugStream();

    // Help menu
    void showAbout();

private:
    void loadAgfsFromDir(QString agfDir);

    Ui::MainWindow *ui;

    ads::CDockManager* dockManager_;
    LensDataManagerDock* lens_data_manager_;

    std::shared_ptr<OpticalSystem> opt_sys_;
    std::string agf_dir_path_;

};
#endif // MAINWINDOW_H
