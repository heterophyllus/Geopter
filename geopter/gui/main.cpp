#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "MainWindow.h"

#include <QApplication>
#include <QtWidgets/QStyleFactory>
#include <QFile>

#include "PythonQt.h"
#include "PythonQt_QtAll.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // set the stylesheet
    QFile styleSheetFile("data/QSS/Diffnes/Diffnes.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);


    // Set monospaced font to align text
#ifdef _WIN64
    QFont fnt("monospace",12);
    a.setFont(fnt);
#elif __APPLE__
    QFont fnt("monaco",12);
    a.setFont(fnt);
#endif


    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    PythonQtObjectPtr mainModule = PythonQt::self()->getMainModule();

    MainWindow w; // This line must be after PythonQt::init()
    mainModule.addObject("app", &w);

    w.show();

    return a.exec();
}
