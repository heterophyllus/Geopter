#ifndef PLOTVIEWDOCK_H
#define PLOTVIEWDOCK_H

#include <memory>

#include "DockWidget.h"
#include "Analysis/renderer_qcp.h"

class QCustomPlot;

/** This dock widget is used to show an analysis result in 2d drawing */
class PlotViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit PlotViewDock(QString label, QWidget *parent = nullptr);
    ~PlotViewDock();

    QCustomPlot* customPlot();

public slots:
    void showSettingDlg();

    /** Save to .png file */
    void saveToFile();

    virtual void updatePlot();

protected:
    QCustomPlot* m_customPlot;
    QToolBar* m_toolbar;
    RendererQCP *m_renderer;
    std::unique_ptr<QDialog> m_settingDlgPtr;

};

#endif // PLOTVIEWDOCK_H
