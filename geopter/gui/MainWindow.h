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

#include "qdebugstream.h"

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


    // Help menu
    void showAbout();

private:    
    template<class T>
    void showAnalysisPlot(QString dockTitleBase);

    template<class T>
    void showAnalysisText(QString dockTitleBase);

    QString createDockTitleWithNumber(QString dockTitleBase);

    void loadAgfsFromDir(QString agfDir);

    Ui::MainWindow *ui;
    ads::CDockManager* m_dockManager;
    SystemEditorDock* m_systemEditorDock;
    PythonQtScriptingConsole* m_pyConsole;
    QTextEdit* m_stdoutText;
    QTextEdit* m_stderrText;
    QDebugStream *m_qout;
    QDebugStream *m_qerr;
    std::shared_ptr<OpticalSystem> opt_sys_;

};



#endif // MAINWINDOW_H
