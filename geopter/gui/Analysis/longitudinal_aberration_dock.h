#ifndef LONGITUDINAL_ABERRATION_DOCK_H
#define LONGITUDINAL_ABERRATION_DOCK_H

#include <memory>
#include "plot_view_dock.h"

#include "optical.h"
using namespace geopter;


class LongitudinalAberrationDock : public PlotViewDock
{
    Q_OBJECT

public:
    LongitudinalAberrationDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~LongitudinalAberrationDock();

    void setOpticalSystem(OpticalSystem* sys);
    void updatePlot() override;

private:
    OpticalSystem* m_opticalSystem;

    double m_plotScale;
};

#endif // ANALYSISSETTINGDIALOG_H
