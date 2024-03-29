#include "analysis/ray_aberration.h"

using namespace geopter;

RayAberration::RayAberration(OpticalSystem* opt_sys)
{
    opt_sys_ = opt_sys;
    num_fld_ = opt_sys_->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    num_wvl_ = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    ref_wvl_val_ = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    ref_wvl_idx_ = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceIndex();
}

RayAberration::~RayAberration()
{
    opt_sys_ = nullptr;
}
