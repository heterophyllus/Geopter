#ifndef PLOTVIEWDOCK_H
#define PLOTVIEWDOCK_H

#include <memory>

#include "DockWidget.h"


class QCustomPlot;

/** This dock widget is used to show an analysis result in 2d drawing */
class PlotViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit PlotViewDock(QString label, QWidget *parent = nullptr);
    ~PlotViewDock();

    void possessDlg(std::unique_ptr<QDialog> dlg);

    QCustomPlot* customPlot();

public slots:
    void showSettingDlg();

    /** Save to .png file */
    void saveToFile();

private:
    QCustomPlot* customPlot_;
    QToolBar* toolbar_;

    std::unique_ptr<QDialog> settingDlgPtr_;
};

#endif // PLOTVIEWDOCK_H
