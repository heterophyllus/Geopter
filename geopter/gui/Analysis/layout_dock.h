#ifndef LAYOUTDOCK_H
#define LAYOUTDOCK_H

#include "plot_view_dock.h"

#include "optical.h"
using namespace geopter;

class LayoutDock : public PlotViewDock
{
    Q_OBJECT

public:
    LayoutDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~LayoutDock();

    void updatePlot() override;

private:
    OpticalSystem* m_opticalSystem;

    bool m_doDrawRefRays;
    bool m_doDrawFanRays;
    int m_numberOfRays;
};

#endif // LAYOUTDOCK_H
