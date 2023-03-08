#include "analysis/ray_aberration.h"

using namespace geopter;

RayAberration::RayAberration(OpticalSystem* opt_sys)
{
    opt_sys_ = opt_sys;
    num_fld_ = opt_sys_->optical_spec()->field_of_view()->field_count();
    num_wvl_ = opt_sys_->optical_spec()->spectral_region()->number_of_wavelengths();
    ref_wvl_val_ = opt_sys_->optical_spec()->spectral_region()->reference_wavelength();
    ref_wvl_idx_ = opt_sys_->optical_spec()->spectral_region()->reference_index();
}

RayAberration::~RayAberration()
{
    opt_sys_ = nullptr;
}
