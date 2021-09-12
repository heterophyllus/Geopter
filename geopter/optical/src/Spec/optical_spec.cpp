#define _USE_MATH_DEFINES
#include <math.h>

#include "Spec/optical_spec.h"

#include "System/optical_system.h"

#include "Assembly/optical_assembly.h"

#include "Spec/pupil_spec.h"
#include "Spec/field_spec.h"
#include "Spec/wvl_spec.h"

#include "Sequential/sequential_trace.h"

#include "Paraxial/paraxial_model.h"



using namespace geopter;

OpticalSpec::OpticalSpec(OpticalSystem* sys) :
    opt_sys_(sys),
    do_aiming_(true)
{
    spectral_region_ = std::make_unique<WvlSpec>();
    pupil_           = std::make_unique<PupilSpec>();
    field_of_view_   = std::make_unique<FieldSpec>();
}

OpticalSpec::~OpticalSpec()
{
    spectral_region_.reset();
    pupil_.reset();
    field_of_view_.reset();
}

PupilSpec* OpticalSpec::pupil_spec()
{
    return pupil_.get();
}

WvlSpec* OpticalSpec::spectral_region()
{
    return spectral_region_.get();
}

FieldSpec* OpticalSpec::field_of_view()
{
    return field_of_view_.get();
}


void OpticalSpec::update_model()
{
    //_spectral_region->update_model();
    pupil_->update_model();

    if(do_aiming_ && (opt_sys_->optical_assembly()->surface_count() > 2))
    {
        int field_count = field_of_view_->field_count();
        int ref_wi = spectral_region_->reference_index();

        SequentialTrace *tracer = new SequentialTrace(opt_sys_);

        for(int fi = 0; fi < field_count; fi++){
            auto aim_pt = tracer->aim_chief_ray(fi, ref_wi);
            field_of_view_->field(fi)->set_aim_pt(aim_pt);
        }

        delete tracer;
    }

    
}


void OpticalSpec::clear()
{
    spectral_region_->clear();
    field_of_view_->clear();
}

void OpticalSpec::print(std::ostringstream &oss)
{
    oss << "Pupil Specs..." << std::endl;
    pupil_->print(oss);

    oss << "Wavelengths..." << std::endl;
    spectral_region_->print(oss);

    oss << "Fields..." << std::endl;
    field_of_view_->print(oss);

}
