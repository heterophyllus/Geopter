#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <iomanip>
#include <limits>

#include "Sequential/sequential_model.h"
#include "Sequential/sequential_trace.h"

#include "Spec/optical_spec.h"
#include "Spec/field_spec.h"
#include "Spec/wvl_spec.h"

#include "System/optical_system.h"

using namespace geopter;

SequentialModel::SequentialModel(OpticalSystem* sys) :
    opt_sys_(sys)
{
    num_fld_ = opt_sys_->optical_spec()->field_of_view()->field_count();
    num_wvl_ = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
}

SequentialModel::~SequentialModel()
{
    opt_sys_ = nullptr;
}


void SequentialModel::update_model()
{
    num_fld_ = opt_sys_->optical_spec()->field_of_view()->field_count();
    num_wvl_ = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();

    update_reference_rays();
}


Ray SequentialModel::reference_ray(int ri, int fi, int wi) const
{
    switch (ri)
    {
    case 1:
        return ref_rays1_[to_ray_index(fi, wi)];
        break;
    case 2:
        return ref_rays2_[to_ray_index(fi, wi)];
        break;
    case 3:
        return ref_rays3_[to_ray_index(fi, wi)];
        break;
    case 4:
        return ref_rays4_[to_ray_index(fi, wi)];
        break;
    case 5:
        return ref_rays5_[to_ray_index(fi, wi)];
        break;
    default:
        throw "Out of range error";
        break;
    }
}


void SequentialModel::update_reference_rays()
{
    int num_rays = num_fld_*num_wvl_;

    ref_rays1_.clear(); ref_rays1_.reserve(num_rays);
    ref_rays2_.clear(); ref_rays2_.reserve(num_rays);
    ref_rays3_.clear(); ref_rays3_.reserve(num_rays);
    ref_rays4_.clear(); ref_rays4_.reserve(num_rays);
    ref_rays5_.clear(); ref_rays5_.reserve(num_rays);

    SequentialTrace* tracer = new SequentialTrace(opt_sys_);

    for(int fi = 0; fi < num_fld_; fi++) {
        for(int wi = 0; wi < num_wvl_; wi++) {
            ref_rays1_.push_back( tracer->trace_pupil_ray(Eigen::Vector2d({ 0.0,  0.0 }), fi, wi) );
            ref_rays2_.push_back( tracer->trace_pupil_ray(Eigen::Vector2d({ 0.0,  1.0 }), fi, wi) );
            ref_rays3_.push_back( tracer->trace_pupil_ray(Eigen::Vector2d({ 0.0, -1.0 }), fi, wi) );
            ref_rays4_.push_back( tracer->trace_pupil_ray(Eigen::Vector2d({ 1.0,  0.0 }), fi, wi) );
            ref_rays5_.push_back( tracer->trace_pupil_ray(Eigen::Vector2d({-1.0,  0.0 }), fi, wi) );

            // set name
            
        }
    }

    delete tracer;
}

void SequentialModel::print(std::ostringstream &oss)
{
    oss << "Reference Rays" << std::endl;
    oss << "Wavelength Index: " << ref_wvl_idx << std::endl;
    oss << std::endl;

    for(int fi = 0; fi < num_fld_; fi++) {
        oss << "Field: " << fi << std::endl;

        oss << "Chief Ray" << std::endl;
        ref_rays1_[to_ray_index(fi, ref_wvl_idx)].print(oss);
        oss << std::endl;

        oss << "Upper Marginal Ray" << std::endl;
        ref_rays2_[to_ray_index(fi, ref_wvl_idx)].print(oss);
        oss << std::endl;

        oss << "Lower Marginal Ray" << std::endl;
        ref_rays3_[to_ray_index(fi, ref_wvl_idx)].print(oss);
        oss << std::endl;

        oss << std::endl;
    }
}


