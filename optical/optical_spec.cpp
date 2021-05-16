#define _USE_MATH_DEFINES
#include <math.h>

#include "optical_spec.h"

#include "optical_model.h"
#include "pupil_spec.h"
#include "field_spec.h"
#include "wvl_spec.h"
#include "sequential_model.h"
#include "paraxial_model.h"
#include "trace.h"



using namespace geopter;

OpticalSpec::OpticalSpec(OpticalModel* opt_model) :
    opt_model_(opt_model),
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

    double wvl = spectral_region_->reference_wvl();

    if(do_aiming_ && (opt_model_->seq_model()->surface_count() > 2)){
        int field_count = field_of_view_->field_count();
        for(int i = 0; i < field_count; i++){
            auto fld = field_of_view_->field(i);
            auto aim_pt = Trace::aim_chief_ray(*opt_model_, *fld, wvl);
            fld->set_aim_pt(aim_pt);
        }
    }
}


Eigen::Vector3d OpticalSpec::obj_coord(const Field& fld)
{
    Eigen::Vector3d obj_pt;

    //auto fov = _field_of_view;
    //auto fod = _parax_data.fod;
    opt_model_->paraxial_model()->compute_first_order();
    auto fod = opt_model_->paraxial_model()->first_order_data();

    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;


    switch (field_of_view_->field_type())
    {
    case FieldType::OBJ_ANG:
        ang_dg = Eigen::Vector3d({fld.x(), fld.y(), 0.0});
        dir_tan(0) = tan(ang_dg(0) * M_PI/180.0);
        dir_tan(1) = tan(ang_dg(1) * M_PI/180.0);
        dir_tan(2) = tan(ang_dg(2) * M_PI/180.0);
        obj_pt = -(fod.obj_dist + fod.enp_dist)*dir_tan;
        break;

    case FieldType::OBJ_HT:
        obj_pt(0) = fld.x();
        obj_pt(1) = fld.y();
        obj_pt(2) = 0.0;
        break;

    case FieldType::IMG_HT:
        img_pt = Eigen::Vector3d({fld.x(), fld.y(), 0.0});
        obj_pt = fod.red*img_pt;
        break;

    default:
        obj_pt = Eigen::Vector3d::Zero(3);
    }

    return obj_pt;
}
