#include "Analysis/layout_dock.h"
#include "Analysis/layout_dialog.h"

LayoutDock::LayoutDock(QString label, OpticalSystem* sys,  QWidget *parent) :
    PlotViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = std::make_unique<LayoutDialog>(this);
}

LayoutDock::~LayoutDock()
{

}

void LayoutDock::updatePlot()
{
    m_renderer->clear();
    m_renderer->set_mouse_interaction(true);

    dynamic_cast<LayoutDialog*>(m_settingDlgPtr.get())->getSettings(&m_doDrawRefRays, &m_doDrawFanRays, &m_numberOfRays);

    Layout *layout = new Layout(m_opticalSystem, m_renderer);

    layout->draw_elements();
    if(m_doDrawRefRays){
        layout->draw_reference_rays();
    }
    if(m_doDrawFanRays){
        layout->draw_fan_rays(m_numberOfRays);
    }
    layout->update();

    delete layout;
}
