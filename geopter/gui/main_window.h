#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "SystemEditor/system_editor_dock.h"

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

    void showPreference();

    // Analysis menu
    // Preference
    void showPrescription();

    // Draw
    void showLayout();

    // Paraxial
    void showFirstOrderData();
    void showParaxialRayTrace();

    // Ray and Spot
    void showSingleRayTrace();
    void showTransverseRayFan();
    void showLongitudinal();

    // Tool menu
    void showDebugStream();

    // Help menu
    void showAbout();

private:
    void loadAgfsFromDir(QString agfDir);

    Ui::MainWindow *ui;

    ads::CDockManager* m_dockManager;

    SystemEditorDock* m_systemEditorDock;

    std::shared_ptr<OpticalSystem> opt_sys_;
    std::string agf_dir_path_;

};
#endif // MAINWINDOW_H
