#ifndef PLOTVIEWDOCK_H
#define PLOTVIEWDOCK_H

#include <memory>
#include <QToolBar>

#include "qcustomplot.h"

#include "DockWidget.h"
#include "AnalysisDlg/AnalysisSettingDlg.h"

#include "optical.h"
using namespace geopter;


/** This dock widget is used to show an analysis result in 2d drawing */
class PlotViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit PlotViewDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~PlotViewDock();

    QCustomPlot* customPlot();

    template<class D> void createSettingDialog() {
        m_settingDlgPtr = std::make_unique<D>(m_opticalSystem, this);
    }

public slots:
    void showSettingDlg();

    /** Save to .png file */
    void saveToFile();

    virtual void updateContent();

protected:
    QCustomPlot* m_customPlot;
    QToolBar* m_toolbar;
    std::unique_ptr<AnalysisSettingDlg> m_settingDlgPtr;
    OpticalSystem *m_opticalSystem;

};

#endif // PLOTVIEWDOCK_H
