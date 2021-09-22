#ifndef TRANSVERSERAYFANDOCK_H
#define TRANSVERSERAYFANDOCK_H

#include "plot_view_dock.h"

#include "optical.h"
using namespace geopter;

class TransverseRayFanDock : public PlotViewDock
{
    Q_OBJECT

public:
    TransverseRayFanDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~TransverseRayFanDock();

    void updatePlot() override;

private:
    OpticalSystem* m_opticalSystem;
};

#endif // TRANSVERSERAYFANDOCK_H
