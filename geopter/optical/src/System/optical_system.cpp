/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

//============================================================================
/// \file   optical_system.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include "nlohmann/json.hpp"

#include "Eigen/Core"
#include "Eigen/Dense"

#include "system/optical_system.h"

#include "paraxial/paraxial_trace.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"

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



void OpticalSystem::clear()
{
    title_ = "";
    note_ = "";
    opt_assembly_->clear();
    opt_spec_->clear();
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
    title_ = "";
    note_ = "";
    opt_spec_->create_minimum_spec();
    opt_assembly_->create_minimun_assembly();
    update_model();
}


void OpticalSystem::add_surface_and_gap(double r, double t, std::string mat_name)
{
    auto m = material_lib_->find(mat_name);

    opt_assembly_->add_surface_and_gap();
    opt_assembly_->current_surface()->profile()->set_cv(1.0/r);
    opt_assembly_->current_gap()->set_thi(t);
    opt_assembly_->current_gap()->set_material(m.get());
}

std::shared_ptr<ParaxialRay> OpticalSystem::axial_ray(int wi) const
{
    return ax_rays_[wi];
}

std::shared_ptr<ParaxialRay> OpticalSystem::principle_ray(int wi) const
{
    return pr_rays_[wi];
}

FirstOrderData OpticalSystem::first_order_data() const
{
    return fod_;
}

std::shared_ptr<Ray> OpticalSystem::reference_ray(int ri, int fi, int wi) const
{
    switch (ri)
    {
    case ReferenceRay::ChiefRay:
        return chief_rays_[to_ray_index(fi, wi)];
        break;
    case ReferenceRay::MeridionalUpperRay:
        return meridional_upper_rays_[to_ray_index(fi, wi)];
        break;
    case ReferenceRay::MeridionalLowerRay:
        return meridional_lower_rays_[to_ray_index(fi, wi)];
        break;
    case ReferenceRay::SagittalUpperRay:
        return sagittal_upper_rays_[to_ray_index(fi, wi)];
        break;
    case ReferenceRay::SagittalLowerRay:
        return sagittal_lower_rays_[to_ray_index(fi, wi)];
        break;
    default:
        throw std::out_of_range("Ray index out of range: OpticalSystem::reference_ray()");
        break;
    }
}

void OpticalSystem::update_aim_pt()
{
    if(opt_assembly_->surface_count() > 2)
    {
        SequentialTrace *tracer = new SequentialTrace(this);
        tracer->set_apply_vig(true);
        tracer->set_aperture_check(false);

        for(int fi = 0; fi < num_fld_; fi++){
            Field* fld = opt_spec_->field_of_view()->field(fi);
            try{
                auto aim_pt = tracer->aim_chief_ray(fld, ref_wvl_val_);
                opt_spec_->field_of_view()->field(fi)->set_aim_pt(aim_pt);
            }catch(TraceRayAimingFailedError &e){
                std::cerr << "Ray aiming failed at field " << fi << std::endl;
                continue;
            }
        }

        delete tracer;
    }
}

void OpticalSystem::update_object_coords()
{
    Eigen::Vector3d obj_pt;

    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    const int field_type = opt_spec_->field_of_view()->field_type();

    for(int fi = 0; fi < num_fld_; fi++){
        Field* fld = opt_spec_->field_of_view()->field(fi);
        double fld_x = fld->x();
        double fld_y = fld->y();

        switch (field_type)
        {
        case FieldType::OBJ_ANG:
            ang_dg = Eigen::Vector3d({fld_x, fld_y, 0.0});
            dir_tan(0) = tan(ang_dg(0) * M_PI/180.0);
            dir_tan(1) = tan(ang_dg(1) * M_PI/180.0);
            dir_tan(2) = tan(ang_dg(2) * M_PI/180.0);
            obj_pt = -dir_tan*(fod_.obj_dist + fod_.enp_dist);
            break;

        case FieldType::OBJ_HT:
            obj_pt(0) = fld_x;
            obj_pt(1) = fld_y;
            obj_pt(2) = 0.0;
            break;

        case FieldType::IMG_HT:
            img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
            obj_pt = fod_.red*img_pt;
            break;

        default:
            obj_pt = Eigen::Vector3d::Zero(3);
        }

        fld->set_object_coord(obj_pt);
    }
}


void OpticalSystem::update_semi_diameters()
{
    const int num_srf = opt_assembly_->surface_count();

    // initialize all surface
    for(int si = 0; si < num_srf; si++) {
        opt_assembly_->surface(si)->set_semi_diameter(0.0);
    }

    // update semi diameter
    std::shared_ptr<Ray> chief_ray, mer_upper_ray, mer_lower_ray, sag_upper_ray, sag_lower_ray;


    for(int fi = 0; fi < num_fld_; fi++) {

        try{
            chief_ray     = reference_ray(ReferenceRay::ChiefRay,           fi, ref_wvl_idx_);
            mer_upper_ray = reference_ray(ReferenceRay::MeridionalUpperRay, fi, ref_wvl_idx_);
            mer_lower_ray = reference_ray(ReferenceRay::MeridionalLowerRay, fi, ref_wvl_idx_);
            sag_upper_ray = reference_ray(ReferenceRay::SagittalUpperRay,   fi, ref_wvl_idx_);
            sag_lower_ray = reference_ray(ReferenceRay::SagittalLowerRay,   fi, ref_wvl_idx_);
        }catch(std::out_of_range &e){
            std::cout << e.what() << std::endl;
            continue;
        }catch(...){
            std::cout << "undefined error in OpticalSystem::update_semidiameter" << std::endl;
        }

        int ray_size = std::min({chief_ray->size(), mer_upper_ray->size(), mer_lower_ray->size(), sag_upper_ray->size(), sag_lower_ray->size()});

        for(int si = 0; si < ray_size; si++) {

            double chief_ray_ht = 0.0;
            double mer_upper_ray_ht = 0.0;
            double mer_lower_ray_ht = 0.0;
            double sag_upper_ray_ht = 0.0;
            double sag_lower_ray_ht = 0.0;
            double ray_ht_for_cur_fld = 0.0;

            try{
                chief_ray_ht     = chief_ray->at(si)->height();
                mer_upper_ray_ht = mer_upper_ray->at(si)->height();
                mer_lower_ray_ht = mer_lower_ray->at(si)->height();
                sag_upper_ray_ht = sag_upper_ray->at(si)->height();
                sag_lower_ray_ht = sag_lower_ray->at(si)->height();
            }
            catch(std::out_of_range &e){
                std::cout << e.what() << std::endl;
                std::cout << "Ray out of range: OpticalSystem::update_semi_diameters()" << std::endl;
                break;
            }
            catch(...){
                std::cout << "Ray out of range: OpticalSystem::update_semi_diameters()" << std::endl;
                break;
            }

            ray_ht_for_cur_fld = std::max({chief_ray_ht, mer_upper_ray_ht, mer_lower_ray_ht, sag_upper_ray_ht, sag_lower_ray_ht});

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

    ax_rays_.clear();
    pr_rays_.clear();

    for(int wi = 0; wi < num_wvl_; wi++)
    {
        double wvl = opt_spec_->spectral_region()->wvl(wi)->value();
        auto ax_ray_for_wvl = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        ax_ray_for_wvl->set_name("axial ray traced with W" + std::to_string(wi));

        auto pr_ray_for_wvl = tracer->trace_paraxial_ray_from_object(ybar0, ubar0, wvl);
        pr_ray_for_wvl->set_name("principle ray traced with W" + std::to_string(wi));

        ax_rays_.push_back(ax_ray_for_wvl);
        pr_rays_.push_back(pr_ray_for_wvl);
    }

    delete tracer;

    double n_0 = opt_assembly_->gap(0)->material()->rindex(ref_wvl_val_);
    double n_k = opt_assembly_->image_space_gap()->material()->rindex(ref_wvl_val_);

    auto ax_ray = ax_rays_[ref_wvl_idx_];
    auto pr_ray = pr_rays_[ref_wvl_idx_];

    std::shared_ptr<ParaxialRay> p_ray = p_ray_;
    std::shared_ptr<ParaxialRay> q_ray = q_ray_;

    double ak1 = p_ray->at(img)->y();
    double bk1 = q_ray->at(img)->y();
    double ck1 = n_k*p_ray->at(img)->u_prime();
    double dk1 = n_k*q_ray->at(img)->u_prime();

    // fill in the content of first order data
    double y1 = ax_ray->at(1)->y();
    double ubar0_prime = pr_ray->at(0)->u_prime();
    double ybar1 = pr_ray->at(1)->y();
    double u0_prime = ax_ray->at(0)->u_prime();
    fod_.opt_inv = n_0 * ( y1*ubar0_prime - ybar1*u0_prime );
    //fod_.opt_inv = n_0 * ( ax_ray->at(1)->y()*pr_ray->at(0)->u_prime() - pr_ray->at(1)->y()*ax_ray->at(0)->u_prime() );
    fod_.obj_dist = opt_assembly_->gap(0)->thi();
    fod_.img_dist = opt_assembly_->image_space_gap()->thi();
    fod_.efl = -1.0/ck1;
    fod_.pp1 = (dk1 - 1.0)*(n_0/ck1);
    fod_.ppk = (p_ray->at(img-1)->y() - 1.0)*(n_k/ck1);
    fod_.ffl = fod_.pp1 - fod_.efl;
    fod_.bfl = fod_.efl - fod_.ppk;
    fod_.fno = -1.0/(2.0*n_k*ax_ray->at(img)->u_prime());

    fod_.m = ak1 + ck1*fod_.img_dist/n_k;
    fod_.red = dk1 + ck1*fod_.obj_dist;

    fod_.n_obj = n_0;
    fod_.n_img = n_k;

    fod_.img_ht = -fod_.opt_inv/(n_k*ax_ray->at(img)->u_prime());
    fod_.obj_ang = atan(pr_ray->at(0)->u_prime()) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray->at(0)->u_prime();
    try {
        fod_.enp_dist = -ybar1/nu_pr0;
        fod_.enp_radius = fabs(fod_.opt_inv/nu_pr0);
        if(std::isnan(fod_.enp_dist)){
            fod_.enp_dist = 0.0;
        }
        if(std::isnan(fod_.enp_radius)){
            fod_.enp_radius = 0.0;
        }
    }  catch (...) {
        fod_.enp_dist = 0.0;
        fod_.enp_radius = 0.0;
    }

    try {
        fod_.exp_dist = -(pr_ray->at(img)->y()/pr_ray->at(img)->u_prime() - fod_.img_dist);
        fod_.exp_radius = fabs( fod_.opt_inv/(n_k*pr_ray->at(img)->u_prime()) );
        if(std::isnan(fod_.exp_dist)){
            fod_.exp_dist = 0.0;
        }
        if(std::isnan(fod_.exp_radius)){
            fod_.exp_radius = 0.0;
        }
    }  catch (...) {
        fod_.exp_dist = 0.0;
        fod_.exp_radius = 0.0;
    }


    fod_.obj_na = n_0*sin( atan(ax_ray->at(0)->u_prime()) );
    fod_.img_na = n_k*sin( atan(ax_ray->at(img)->u_prime()) );
}

void OpticalSystem::update_reference_rays()
{
    int num_rays = num_fld_*num_wvl_;

    chief_rays_.clear(); chief_rays_.reserve(num_rays);
    meridional_upper_rays_.clear(); meridional_upper_rays_.reserve(num_rays);
    meridional_lower_rays_.clear(); meridional_lower_rays_.reserve(num_rays);
    sagittal_upper_rays_.clear(); sagittal_upper_rays_.reserve(num_rays);
    sagittal_lower_rays_.clear(); sagittal_lower_rays_.reserve(num_rays);

    Eigen::Vector2d pupil_chief({0.0,0.0});
    Eigen::Vector2d pupil_upper_mer({0.0,1.0});
    Eigen::Vector2d pupil_lower_mer({0.0,-1.0});
    Eigen::Vector2d pupil_upper_sag({1.0,0.0});
    Eigen::Vector2d pupil_lower_sag({-1.0,0.0});

    SequentialTrace* tracer = new SequentialTrace(this);
    tracer->set_aperture_check(false);
    tracer->set_apply_vig(true);

    for(int fi = 0; fi < num_fld_; fi++) {
        Field* fld = opt_spec_->field_of_view()->field(fi);
        for(int wi = 0; wi < num_wvl_; wi++) {

            double wvl = opt_spec_->spectral_region()->wvl(wi)->value();

            try{
                auto ray = tracer->trace_pupil_ray(pupil_chief, fld, wvl);
                chief_rays_.push_back(ray);
            }catch(TraceError &e){
                std::cerr << "Failed to trace chief ray at field " << fi << ", wavelength " << wi << std::endl;
                std::cerr << "    " << e.cause_str() << std::endl;
                chief_rays_.push_back( e.ray() );
            }

            try{
                auto ray = tracer->trace_pupil_ray(pupil_upper_mer, fld, wvl);
                meridional_upper_rays_.push_back(ray);
            }catch(TraceError &e){
                std::cerr << "Failed to trace meridional upper ray at field " << fi << ", wavelength " << wi << std::endl;
                std::cerr << "    " << e.cause_str() << std::endl;
                meridional_upper_rays_.push_back(e.ray());
            }

            try{
                auto ray = tracer->trace_pupil_ray(pupil_lower_mer, fld, wvl);
                meridional_lower_rays_.push_back(ray);
            }catch(TraceError &e){
                std::cerr << "Failed to trace meridional lower ray at field " << fi << ", wavelength " << wi << std::endl;
                std::cerr << "    " << e.cause_str() << std::endl;
                meridional_lower_rays_.push_back(e.ray());
            }

            try{
                auto ray = tracer->trace_pupil_ray(pupil_upper_sag, fld, wvl);
                sagittal_upper_rays_.push_back(ray);
            }catch(TraceError &e){
                std::cerr << "Failed to trace sagittal upper ray at field " << fi << ", wavelength " << wi << std::endl;
                std::cerr << "    " << e.cause_str() << std::endl;
                sagittal_upper_rays_.push_back(e.ray());
            }

            try{
                auto ray = tracer->trace_pupil_ray(pupil_lower_sag, fld, wvl);
                sagittal_lower_rays_.push_back( ray );
            }catch(TraceError &e){
                std::cerr << "Failed to trace sagittal lower ray at field " << fi << ", wavelength " << wi << std::endl;
                std::cerr << "    " << e.cause_str() << std::endl;
                sagittal_lower_rays_.push_back(e.ray());
            }

            // set name

        }
    }

    delete tracer;
}

void OpticalSystem::set_vignetting_factors()
{
    SequentialTrace *tracer = new SequentialTrace(this);

    for(int fi = 0; fi < num_fld_; fi++){
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

    num_fld_ = opt_spec_->field_of_view()->field_count();
    num_wvl_ = opt_spec_->spectral_region()->wvl_count();
    ref_wvl_idx_ = opt_spec_->spectral_region()->reference_index();
    ref_wvl_val_ = opt_spec_->spectral_region()->reference_wvl();

    update_paraxial_data();
    update_object_coords();
    update_aim_pt();
    update_reference_rays();
    update_semi_diameters();
}





void OpticalSystem::save_to_file(const std::string &filepath)
{
    nlohmann::json json_data;

    json_data["Title"] = title();
    json_data["Note"]  = note();

    /* PupilSpec */
    auto pupil = opt_spec_->pupil_spec();
    json_data["Spec"]["Pupil"]["Type"]  = pupil->pupil_type();
    json_data["Spec"]["Pupil"]["Value"] = pupil->value();


    /* FieldSpec */
    json_data["Spec"]["Field"]["Type"] = opt_spec_->field_of_view()->field_type();

    int num_flds = opt_spec_->field_of_view()->field_count();

    for(int fi = 0; fi < num_flds; fi++){
        auto fld = opt_spec_->field_of_view()->field(fi);
        json_data["Spec"]["Field"]["X"].push_back(fld->x());
        json_data["Spec"]["Field"]["Y"].push_back(fld->y());
        json_data["Spec"]["Field"]["Weight"].push_back(fld->weight());
        json_data["Spec"]["Field"]["VUX"].push_back(fld->vux());
        json_data["Spec"]["Field"]["VLX"].push_back(fld->vlx());
        json_data["Spec"]["Field"]["VUY"].push_back(fld->vuy());
        json_data["Spec"]["Field"]["VLY"].push_back(fld->vly());

        json_data["Spec"]["Field"]["Color"].push_back({fld->render_color().r, fld->render_color().g, fld->render_color().b, fld->render_color().a});
    }


    /* WvlSpec */
    json_data["Spec"]["Wvl"]["RefIndex"] = opt_spec_->spectral_region()->reference_index();

    const int num_wvls = opt_spec_->spectral_region()->wvl_count();

    for(int wi = 0; wi < num_wvls; wi++){
        auto wvl = opt_spec_->spectral_region()->wvl(wi);

        json_data["Spec"]["Wvl"]["Value"].push_back(wvl->value());
        json_data["Spec"]["Wvl"]["Weight"].push_back(wvl->weight());

        json_data["Spec"]["Wvl"]["Color"].push_back({ wvl->render_color().r, wvl->render_color().g, wvl->render_color().b, wvl->render_color().a });
    }

    /* assembly data */
    const int num_srfs = opt_assembly_->surface_count();

    int stop_surface = optical_assembly()->stop_index();
    json_data["Assembly"]["Stop"] = stop_surface;

    for( int ci = 0; ci < num_srfs; ci++){

        std::string cur_idx = std::to_string(ci);

        auto s = optical_assembly()->surface(ci);
        auto g = optical_assembly()->gap(ci);

        // ---> surface attributes
        std::string surface_label = s->label();
        std::string surface_type  = s->profile()->name();
        double      cv            = s->profile()->cv();


        json_data["Assembly"][cur_idx]["Label"]     = surface_label;
        json_data["Assembly"][cur_idx]["Type"]      = surface_type;
        json_data["Assembly"][cur_idx]["Curvature"] = cv;

        if(surface_type == "SPH"){
            // do nothing
        }else if(surface_type == "ASP"){
            EvenPolynomial *prf = dynamic_cast<EvenPolynomial*>(s->profile());
            if(prf){
                json_data["Assembly"][cur_idx]["Conic"] = prf->conic();
                for(int ci = 0; ci < prf->coef_count(); ci++){
                    json_data["Assembly"][cur_idx]["Coefs"].push_back( prf->coef(ci) );
                }
            }
        }else if(surface_type == "ODD"){
            OddPolynomial *prf = dynamic_cast<OddPolynomial*>(s->profile());
            if(prf){
                json_data["Assembly"][cur_idx]["Conic"] = prf->conic();
                for(int ci = 0; ci < prf->coef_count(); ci++){
                    json_data["Assembly"][cur_idx]["Coefs"].push_back( prf->coef(ci) );
                }
            }
        }

        if(s->clear_aperture()){
            std::string aperture_type = s->aperture_shape();
            json_data["Assembly"][cur_idx]["Aperture"]["Type"]  = aperture_type;

            if(aperture_type == "Circular"){
                double cir_ap_r = dynamic_cast<Circular*>(s->clear_aperture())->max_dimension();
                json_data["Assembly"][cur_idx]["Aperture"]["Radius"] = cir_ap_r;
            }else if(aperture_type == "Rectangular"){
                // not implemented
            }
        }
        // <--- surface attributes end


        // ---> gap attributes
        double thi;
        std::string mat_name;

        if(g){
            thi = g->thi();
            mat_name = g->material()->name();
        }else{ // assume at image surface
            thi = 0.0;
            mat_name = "";
        }

        json_data["Assembly"][cur_idx]["Thickness"] = thi;
        json_data["Assembly"][cur_idx]["Material"]  = mat_name;
        // <--- gap attributes end

    }

    /* output to file */
    std::ofstream fout(filepath);
    //fout << std::setw(4) << json_data << std::endl;
    fout << json_data.dump(4) << std::endl;

}


void OpticalSystem::load_file(const std::string &filepath)
{
    std::ifstream ifs(filepath);
    if(!ifs){
        std::cerr << "JSON Error: Failed to open json file" << filepath << std::endl;
        return;
    }

    nlohmann::json json_data;
    ifs >> json_data;

    this->clear();

    // ---> title, note
    std::string sys_title = json_data["Title"].get< std::string >();
    set_title(sys_title);

    std::string sys_note = json_data["Note"].get< std::string >();
    set_note(sys_note);

    // ---> Spec start
    if(json_data.find("Spec") == json_data.end()){
        std::cerr << "JSON Error: Wrong format, Spec not found" << std::endl;
    }

    // pupil
    try
    {
        int pupil_type = json_data["Spec"]["Pupil"]["Type"].get<int>();
        double pupil_value = json_data["Spec"]["Pupil"]["Value"].get<double>();
        opt_spec_->pupil_spec()->set_pupil_type(pupil_type);
        opt_spec_->pupil_spec()->set_value(pupil_value);
    }
    catch(...)
    {
        std::cerr << "Failed to find pupil spec" << std::endl;
    }


    using RgbAsVector = std::vector<double>;

    // wavelengths
    try
    {


        int ref_wvl_idx = json_data["Spec"]["Wvl"]["RefIndex"].get<int>();
        opt_spec_->spectral_region()->set_reference_index(ref_wvl_idx);

        std::vector<double> wvl_val  = json_data["Spec"]["Wvl"]["Value"].get< std::vector<double> >();
        std::vector<double> wvl_wt   = json_data["Spec"]["Wvl"]["Weight"].get< std::vector<double> >();

        std::vector< RgbAsVector > wvl_color = json_data["Spec"]["Wvl"]["Color"].get< std::vector<RgbAsVector> >();

        for(int wi = 0; wi < (int)wvl_val.size(); wi++){
            Rgb color(wvl_color[wi][0], wvl_color[wi][1], wvl_color[wi][2], 0.0);
            opt_spec_->spectral_region()->add(wvl_val[wi], wvl_wt[wi],color);
        }
    }
    catch(...)
    {
        std::cerr << "Failed to load wvl spec" << std::endl;
    }

    // fields
    try
    {
        int field_type = json_data["Spec"]["Field"]["Type"].get<int>();
        opt_spec_->field_of_view()->set_field_type(field_type);

        std::vector<double> fld_x   = json_data["Spec"]["Field"]["X"].get< std::vector<double> >();
        std::vector<double> fld_y   = json_data["Spec"]["Field"]["Y"].get< std::vector<double> >();
        std::vector<double> fld_wt  = json_data["Spec"]["Field"]["Weight"].get< std::vector<double> >();
        std::vector<double> fld_vlx = json_data["Spec"]["Field"]["VLX"].get< std::vector<double> >();
        std::vector<double> fld_vux = json_data["Spec"]["Field"]["VUX"].get< std::vector<double> >();
        std::vector<double> fld_vly = json_data["Spec"]["Field"]["VLY"].get< std::vector<double> >();
        std::vector<double> fld_vuy = json_data["Spec"]["Field"]["VUY"].get< std::vector<double> >();

        std::vector< RgbAsVector > fld_color = json_data["Spec"]["Field"]["Color"].get< std::vector<RgbAsVector> >();

        for(int fi = 0; fi < (int)fld_x.size(); fi++){

            Rgb color(fld_color[fi][0], fld_color[fi][1], fld_color[fi][2], 0.0);

            opt_spec_->field_of_view()->add(fld_x[fi], fld_y[fi],
                                                            fld_wt[fi],
                                                            color,
                                                            fld_vuy[fi], fld_vly[fi],
                                                            fld_vux[fi], fld_vux[fi]);
        }
    }
    catch(...)
    {
        std::cerr << "Failed to load field spec" << std::endl;
    }
    // <--- Spec end


    // ---> Assembly start
    if(json_data.find("Assembly") == json_data.end()){
        std::cout << "JSON Error: Wrong format, Assembly not found" << std::endl;
    }

    opt_assembly_->clear();

    int ci = 0;

    while(true){
        std::string cur_idx = std::to_string(ci);

        if( !json_data["Assembly"].contains(cur_idx) ){
            break;
        }

        opt_assembly_->add_surface_and_gap();

        // surface attributes
        auto srf = opt_assembly_->current_surface();

        std::string surf_label = json_data["Assembly"][cur_idx]["Label"].get<std::string>();
        srf->set_label(surf_label);

        std::string surf_type =  json_data["Assembly"][cur_idx]["Type"].get<std::string>();
        double cv= 0.0;
        try
        {
            cv = json_data["Assembly"][cur_idx]["Curvature"].get<double>();
        }
        catch(...)
        {
            cv = 0.0;
        }


        if( surf_type == "SPH" ) {
            srf->set_profile<SurfaceProfile::Type::Sphere>(cv);
        }
        else if( surf_type == "ASP" ) {
            double conic = json_data["Assembly"][cur_idx]["Conic"].get<double>();
            std::vector<double> coefs = json_data["Assembly"][cur_idx]["Coefs"].get< std::vector<double> >();

            srf->set_profile<SurfaceProfile::Type::EvenAsphere>(cv, conic, coefs);
        }
        else if(surf_type == "ODD") {
            double conic = json_data["Assembly"][cur_idx]["Conic"].get<double>();
            std::vector<double> coefs = json_data["Assembly"][cur_idx]["Coefs"].get< std::vector<double> >();

            srf->set_profile<SurfaceProfile::Type::OddAsphere>(cv, conic, coefs);
        }


        // aperture
        if( json_data["Assembly"][cur_idx].contains("Aperture") ){
            std::string aperture_type = json_data["Assembly"][cur_idx]["Aperture"]["Type"].get<std::string>();
            if(aperture_type == "Circular"){
                double cir_ap_r = json_data["Assembly"][cur_idx]["Aperture"]["Radius"].get<double>();
                srf->set_clear_aperture<Aperture::Shape::Circular>(cir_ap_r);
            }
        }

        // gap
        double thi = json_data["Assembly"][cur_idx]["Thickness"].get<double>();
        std::string mat_name = json_data["Assembly"][cur_idx]["Material"].get<std::string>();
        auto mat = material_lib_->find(mat_name);

        auto gap = opt_assembly_->current_gap();
        gap->set_thi(thi);
        gap->set_material(mat.get());

        ci++;
    }

    int stop_idx = json_data["Assembly"]["Stop"].get<int>();
    opt_assembly_->set_stop(stop_idx);

    // <--- Assembly end


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
