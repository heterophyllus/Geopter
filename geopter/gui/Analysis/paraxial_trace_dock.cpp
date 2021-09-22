#include <iomanip>
#include "paraxial_trace_dock.h"
#include "Analysis/paraxial_trace_dialog.h"

ParaxialTraceDock::ParaxialTraceDock(QString label, OpticalSystem* sys,  QWidget *parent) :
    TextViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = std::make_unique<ParaxialTraceDialog>(m_opticalSystem, this);
}

ParaxialTraceDock::~ParaxialTraceDock()
{

}

void ParaxialTraceDock::updateText()
{
    m_opticalSystem->update_model();

    int wi;
    dynamic_cast<ParaxialTraceDialog*>(m_settingDlgPtr.get())->getSettings(&wi);

    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    ParaxialRay ax_ray = m_opticalSystem->axial_ray(wi);
    ParaxialRay pr_ray = m_opticalSystem->principle_ray(wi);

    const int idx_w  = 4;
    const int val_w  = 10;
    const int prec   = 4;
    std::ostringstream oss;

    oss << "Paraxial Trace" << std::endl;
    oss << std::endl;
    oss << "Wavelength: " << wvl << std::endl;
    oss << std::endl;

    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "HMY";
    oss << std::setw(val_w) << std::right << "UMY";
    oss << std::setw(val_w) << std::right << "N*IMY";
    oss << std::setw(val_w) << std::right << "HCY";
    oss << std::setw(val_w) << std::right << "UCY";
    oss << std::setw(val_w) << std::right << "N*ICY";
    oss << std::endl;

    assert( ax_ray.size() == pr_ray.size() );
    for(int i = 0; i < ax_ray.size(); i++) {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).slp;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).n * ax_ray.at(i).aoi;

        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).slp;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).n * pr_ray.at(i).aoi;

        oss << std::endl;
    }

    setStringStreamToText(oss);
}
