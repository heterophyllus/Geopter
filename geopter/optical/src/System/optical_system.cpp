#include <iostream>

#include "Eigen/Core"
#include "Eigen/Dense"

#include "System/optical_system.h"

#include "Assembly/surface_profile.h"
#include "Paraxial/paraxial_trace.h"
#include "Sequential/sequential_trace.h"


using namespace geopter;

OpticalSystem::OpticalSystem() : 
    title_(""),
    note_("")
{
    opt_spec_     = std::make_unique<OpticalSpec>();
    opt_assembly_ = std::make_unique<OpticalAssembly>();
    material_lib_ = std::make_unique<MaterialLibrary>();

    fod_ = FirstOrderData();
}

OpticalSystem::~OpticalSystem()
{
    opt_assembly_.reset();
    opt_spec_.reset();
    material_lib_.reset();
}

std::string OpticalSystem::title() const
{
    return title_;
}

void OpticalSystem::set_title(std::string title)
{
    title_ = title;
}

std::string OpticalSystem::note() const
{
    return note_;
}

void OpticalSystem::set_note(std::string note)
{
    note_ = note;
}

OpticalSpec* OpticalSystem::optical_spec() const
{
    return opt_spec_.get();
}

OpticalAssembly* OpticalSystem::optical_assembly() const
{
    return opt_assembly_.get();
}


MaterialLibrary* OpticalSystem::material_lib() const
{
    return material_lib_.get();
}


void OpticalSystem::create_minimum_system()
{
    opt_spec_->create_minimum_spec();
    opt_assembly_->create_minimun_assembly();
}


void OpticalSystem::add_surface_and_gap(double r, double t, std::string mat_name)
{
    auto s = std::make_shared<Surface>();
    s->profile()->set_cv(1.0/r);

    auto g = std::make_shared<Gap>();
    g->set_thi(t);

    auto m = material_lib_->find(mat_name);
    if(m) {
        g->set_material(m);
    }

    opt_assembly_->add_surface_and_gap(s, g);
}

ParaxialRay OpticalSystem::axial_ray(int wi) const
{
    return ax_rays_[wi];
}

ParaxialRay OpticalSystem::principle_ray(int wi) const
{
    return pr_rays_[wi];
}

FirstOrderData OpticalSystem::first_order_data() const
{
    return fod_;
}

Ray OpticalSystem::reference_ray(int ri, int fi, int wi) const
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

void OpticalSystem::update_aim_pt()
{
    if(opt_assembly_->surface_count() > 2)
    {
        int field_count = opt_spec_->field_of_view()->field_count();
        int ref_wi = opt_spec_->spectral_region()->reference_index();

        SequentialTrace *tracer = new SequentialTrace(this);

        for(int fi = 0; fi < field_count; fi++){
            auto aim_pt = tracer->aim_chief_ray(fi, ref_wi);
            opt_spec_->field_of_view()->field(fi)->set_aim_pt(aim_pt);
        }

        delete tracer;
    }
}


void OpticalSystem::update_semi_diameters()
{
    const int num_srf = opt_assembly_->surface_count();
    const int num_fld = opt_spec_->field_of_view()->field_count();
    const int ref_wvl_idx = opt_spec_->spectral_region()->reference_index();

    // initialize all surface
    for(int si = 0; si < num_srf; si++) {
        opt_assembly_->surface(si)->set_semi_diameter(0.0);
    }

    // update semi diameter
    Ray chief_ray, mer_upper_ray, mer_lower_ray, sag_upper_ray, sag_lower_ray;

    for(int fi = 0; fi < num_fld; fi++) {

        chief_ray     = reference_ray(1,fi,ref_wvl_idx);
        mer_upper_ray = reference_ray(2,fi,ref_wvl_idx);
        mer_lower_ray = reference_ray(3,fi,ref_wvl_idx);
        sag_upper_ray = reference_ray(4,fi,ref_wvl_idx);
        sag_lower_ray = reference_ray(5,fi,ref_wvl_idx);

        for(int si = 0; si < num_srf; si++) {

            double chief_ray_ht     = chief_ray.at(si).height();
            double mer_upper_ray_ht = mer_upper_ray.at(si).height();
            double mer_lower_ray_ht = mer_lower_ray.at(si).height();
            double sag_upper_ray_ht = sag_upper_ray.at(si).height();
            double sag_lower_ray_ht = sag_lower_ray.at(si).height();

            double ray_ht_for_cur_fld = std::max({chief_ray_ht, mer_upper_ray_ht, mer_lower_ray_ht, sag_upper_ray_ht, sag_lower_ray_ht});

            double current_sd = opt_assembly_->surface(si)->semi_diameter();

            if(current_sd < ray_ht_for_cur_fld) {
                opt_assembly_->surface(si)->set_semi_diameter(ray_ht_for_cur_fld);
            }
        }

    }
}


void OpticalSystem::update_paraxial_data()
{
    /**
     * 1. trace paraxial rays
     * 2. compute first order data
     *
     */

    //-----> trace paraxial rays
    ParaxialTrace* tracer = new ParaxialTrace(this);

    // p/q rays
    p_ray_ = tracer->trace_paraxial_ray_parallel_to_axis_at_s1();
    q_ray_ = tracer->trace_paraxial_ray_with_slope_at_s1();


    // ax/pr rays

    // starting coords are computed with reference wavelength and are common for all wavelength
    double y0, u0, ybar0, ubar0;
    tracer->get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    int img = opt_assembly_->image_index();
    int num_wvls = opt_spec_->spectral_region()->wvl_count();

    ax_rays_.clear();
    pr_rays_.clear();

    for(int wi = 0; wi < num_wvls; wi++)
    {
        double wvl = opt_spec_->spectral_region()->wvl(wi)->value();
        ParaxialRay ax_ray_for_wvl = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        ax_ray_for_wvl.set_name("axial ray traced with W" + std::to_string(wi));

        ParaxialRay pr_ray_for_wvl = tracer->trace_paraxial_ray_from_object(ybar0, ubar0, wvl);
        pr_ray_for_wvl.set_name("principle ray traced with W" + std::to_string(wi));

        ax_rays_.push_back(ax_ray_for_wvl);
        pr_rays_.push_back(pr_ray_for_wvl);
    }

    delete tracer;

    int ref_wi = opt_spec_->spectral_region()->reference_index();
    double ref_wvl = opt_spec_->spectral_region()->reference_wvl();

    double n_0 = opt_assembly_->gap(0)->material()->rindex(ref_wvl);
    double n_k = opt_assembly_->image_space_gap()->material()->rindex(ref_wvl);

    ParaxialRay ax_ray = ax_rays_[ref_wi];
    ParaxialRay pr_ray = pr_rays_[ref_wi];

    ParaxialRay p_ray = p_ray_;
    ParaxialRay q_ray = q_ray_;

    double ak1 = p_ray.at(img).ht;
    double bk1 = q_ray.at(img).ht;
    double ck1 = n_k*p_ray.at(img).slp;
    double dk1 = n_k*q_ray.at(img).slp;

    // fill in the content of first order data
    fod_.opt_inv = n_0 * ( ax_ray.at(1).ht*pr_ray.at(0).slp - pr_ray.at(1).ht*ax_ray.at(0).slp );
    fod_.obj_dist = opt_assembly_->gap(0)->thi();
    fod_.img_dist = opt_assembly_->image_space_gap()->thi();
    fod_.efl = -1.0/ck1;
    fod_.pp1 = (dk1 - 1.0)*(n_0/ck1);
    fod_.ppk = (p_ray.at(img-1).ht - 1.0)*(n_k/ck1);
    fod_.ffl = fod_.pp1 - fod_.efl;
    fod_.bfl = fod_.efl - fod_.ppk;
    fod_.fno = -1.0/(2.0*n_k*ax_ray.at(img).slp);

    fod_.m = ak1 + ck1*fod_.img_dist/n_k;
    fod_.red = dk1 + ck1*fod_.obj_dist;

    fod_.n_obj = n_0;
    fod_.n_img = n_k;

    fod_.img_ht = -fod_.opt_inv/(n_k*ax_ray.at(img).slp);
    fod_.obj_ang = atan(pr_ray.at(0).slp) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray.at(0).slp;
    fod_.enp_dist = -pr_ray.at(1).ht/nu_pr0;
    fod_.enp_radius = abs(fod_.opt_inv/nu_pr0);

    fod_.exp_dist = -(pr_ray.at(img).ht/pr_ray.at(img).slp - fod_.img_dist);
    fod_.exp_radius = abs( fod_.opt_inv/(n_k*pr_ray.at(img).slp) );

    fod_.obj_na = n_0*sin( atan(ax_ray.at(0).slp) );
    fod_.img_na = n_k*sin( atan(ax_ray.at(img).slp) );
}

void OpticalSystem::update_reference_rays()
{
    int num_fld = opt_spec_->field_of_view()->field_count();
    int num_wvl = opt_spec_->spectral_region()->wvl_count();
    int num_rays = num_fld*num_wvl;

    ref_rays1_.clear(); ref_rays1_.reserve(num_rays);
    ref_rays2_.clear(); ref_rays2_.reserve(num_rays);
    ref_rays3_.clear(); ref_rays3_.reserve(num_rays);
    ref_rays4_.clear(); ref_rays4_.reserve(num_rays);
    ref_rays5_.clear(); ref_rays5_.reserve(num_rays);

    Eigen::Vector2d pupil_chief({0.0,0.0});
    Eigen::Vector2d pupil_upper_mer({0.0,1.0});
    Eigen::Vector2d pupil_lower_mer({0.0,-1.0});
    Eigen::Vector2d pupil_upper_sag({1.0,0.0});
    Eigen::Vector2d pupil_lower_sag({-1.0,0.0});

    SequentialTrace* tracer = new SequentialTrace(this);
    tracer->set_aperture_check(false);
    tracer->set_apply_vig(true);

    for(int fi = 0; fi < num_fld; fi++) {
        for(int wi = 0; wi < num_wvl; wi++) {
            ref_rays1_.push_back( tracer->trace_pupil_ray(pupil_chief, fi, wi) );
            ref_rays2_.push_back( tracer->trace_pupil_ray(pupil_upper_mer, fi, wi) );
            ref_rays3_.push_back( tracer->trace_pupil_ray(pupil_lower_mer, fi, wi) );
            ref_rays4_.push_back( tracer->trace_pupil_ray(pupil_upper_sag, fi, wi) );
            ref_rays5_.push_back( tracer->trace_pupil_ray(pupil_lower_sag, fi, wi) );

            // set name

        }
    }

    delete tracer;
}

void OpticalSystem::update_vignetting_factors()
{
    SequentialTrace *tracer = new SequentialTrace(this);

    int num_fld = opt_spec_->field_of_view()->field_count();
    for(int fi = 0; fi < num_fld; fi++){
        Field* fld = opt_spec_->field_of_view()->field(fi);
        std::vector<double> vig_factors = tracer->compute_vignetting_factors(*fld);
        double vuy = vig_factors[0];
        double vly = vig_factors[1];
        double vux = vig_factors[2];
        double vlx = vig_factors[3];
        opt_spec_->field_of_view()->field(fi)->set_vuy(vuy);
        opt_spec_->field_of_view()->field(fi)->set_vly(vly);
        opt_spec_->field_of_view()->field(fi)->set_vux(vux);
        opt_spec_->field_of_view()->field(fi)->set_vlx(vlx);
    }

    delete tracer;

}

void OpticalSystem::update_model()
{
    opt_assembly_->update_model();

    update_paraxial_data();
    update_aim_pt();
    update_reference_rays();
    update_semi_diameters();
}



void OpticalSystem::clear()
{
    title_ = "";
    note_ = "";
    opt_assembly_->clear();
    opt_spec_->clear();
}

void OpticalSystem::print(std::ostringstream &oss)
{
    oss << "Title : " << title_ << std::endl;
    oss << std::endl;
    oss << "Note: " << std::endl;
    oss << note_ << std::endl;
    oss << std::endl;

    oss << "Specifications:" << std::endl;
    oss << std::endl;
    opt_spec_->print(oss);

    oss << "Lens Data" << std::endl;
    oss << std::endl;
    opt_assembly_->print(oss);
    oss << std::endl;

}
