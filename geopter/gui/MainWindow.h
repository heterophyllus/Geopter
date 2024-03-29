#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "gui/PythonQtScriptingConsole.h"

#include "SystemEditor/SystemEditorDock.h"
#include "Dock/AnalysisViewDock.h"
#include "qdebugstream.h"

#include "OpticalSystemWrapper/QOpticalSystem.h"
using namespace geopter;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

Q_DECLARE_METATYPE(std::string)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent* event) override;


public slots:
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
    void showOpdFan();
    void showLongitudinal();
    void showFieldCurvature();
    void showChromaticFocusShift();

    void showWavefront();
    void showFFTPSF();
    void showGeoMTF();
    void showFFTMTF();

    // Tool menu
    void showConsole();

    // Help menu
    void showAbout();

    void updateUi();
private:    
    template<class T>
    void showAnalysisPlot(QString dockTitleBase);

    template<class T>
    void showAnalysisText(QString dockTitleBase);

    template<class T>
    void showAnalysisResult(QString dockTitleBase, bool textOnly = false);

    QString createDockTitleWithNumber(QString dockTitleBase);
    void loadAgfsFromDir(QString agfDir);
    void syncUiWithSystem();

    Ui::MainWindow *ui;
    ads::CDockManager* m_dockManager;
    SystemEditorDock* m_systemEditorDock;
    PythonQtScriptingConsole* m_pyConsole;
    QDebugStream *m_qout;
    QDebugStream *m_qerr;
    std::shared_ptr<QOpticalSystem> opt_sys_;

};



#endif // MAINWINDOW_H
