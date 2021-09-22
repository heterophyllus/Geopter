#include "Analysis/transverse_ray_fan_dock.h"
#include "Analysis/transverse_ray_fan_dialog.h"

TransverseRayFanDock::TransverseRayFanDock(QString label, OpticalSystem* sys,  QWidget *parent) :
    PlotViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = std::make_unique<TransverseRayFanDialog>(this);
}

TransverseRayFanDock::~TransverseRayFanDock()
{

}

void TransverseRayFanDock::updatePlot()
{
    m_opticalSystem->update_model();

    double scale;
    int nrd;
    dynamic_cast<TransverseRayFanDialog*>(m_settingDlgPtr.get())->getSettings(&scale, &nrd);

    RayFan *ray_fan = new RayFan(m_opticalSystem, m_renderer);
    ray_fan->plot(scale, nrd);
    delete ray_fan;
}
