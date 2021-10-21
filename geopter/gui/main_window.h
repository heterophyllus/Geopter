#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "PythonQt.h"
#include "PythonQt_QtAll.h"

#include "systemeditor/system_editor_dock.h"

#include "optical.h"
using namespace geopter;


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


public slots:
    // The following functions are used in python scripting
    // The naming conventions imitates to Zemax ZPL



private slots:

    // File menu
    void newFile();
    void saveAs();
    void openFile();

    void showPreference();

    // Edit menu
    void setVignettingFactors();

    // Analysis menu
    // Prescription
    void showPrescription();

    // Draw
    void showLayout();

    // Paraxial
    void showParaxialRayTrace();

    // Ray and Spot
    void showSingleRayTrace();
    void showSpotDiagram();

    // Aberration
    void showTransverseRayFan();
    void showLongitudinal();
    void showFieldCurvature();
    void showChromaticFocusShift();

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

    PythonQtObjectPtr* m_mainContext;

};
#endif // MAINWINDOW_H
