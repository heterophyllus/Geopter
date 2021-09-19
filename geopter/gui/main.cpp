#include "main_window.h"

#include <QApplication>
//#include <QtWidgets/QStyleFactory>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set the stylesheet
    /*
    QFile styleSheetFile("./Eclippy/Eclippy.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);
    */

    // Set monospaced font to align text
#ifdef _WIN64
    QFont fnt("monospace",12);
    a.setFont(fnt);
#elif __APPLE__
    QFont fnt("monaco",12);
    a.setFont(fnt);
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
