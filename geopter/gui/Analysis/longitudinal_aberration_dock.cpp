#include "Analysis/longitudinal_aberration_dock.h"
#include "Analysis/longitudinal_setting_dialog.h"


LongitudinalAberrationDock::LongitudinalAberrationDock(QString label, OpticalSystem* sys,  QWidget *parent):
    PlotViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = std::make_unique<LongitudinalSettingDialog>(this);
}

LongitudinalAberrationDock::~LongitudinalAberrationDock()
{

}

void LongitudinalAberrationDock::setOpticalSystem(OpticalSystem *sys)
{
    m_opticalSystem = sys;
}

void LongitudinalAberrationDock::updatePlot()
{
    m_opticalSystem->update_model();

    m_renderer->set_mouse_interaction(false);
    m_renderer->clear();

    double scale = dynamic_cast<LongitudinalSettingDialog*>(m_settingDlgPtr.get())->getScale();
    Longitudinal lon(m_opticalSystem, m_renderer);
    lon.plot_lsa(scale);
    m_renderer->update();
}
