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
    parax_data_   = std::make_unique<ParaxialData>();
}

OpticalSystem::~OpticalSystem()
{
    opt_assembly_.reset();
    opt_spec_.reset();
    material_lib_.reset();
    parax_data_.reset();
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


void OpticalSystem::initialize()
{
    title_ = "";
    note_ = "";
    opt_spec_->create_minimum_spec();
    opt_assembly_->create_minimun_assembly();
    update_model();
}

void OpticalSystem::update_model()
{
    opt_assembly_->update_model();

    update_fundamental_data();

    // be carefull to the updating order
    update_paraxial_data();
    update_fundamental_data();
    update_optical_spec();
    update_semi_diameters();
}

void OpticalSystem::add_surface_and_gap(double r, double t, std::string mat_name)
{
    auto m = material_lib_->find(mat_name);

    opt_assembly_->add_surface_and_gap();
    opt_assembly_->current_surface()->profile()->set_cv(1.0/r);
    opt_assembly_->current_gap()->set_thi(t);
    opt_assembly_->current_gap()->set_material(m.get());
}

void OpticalSystem::update_fundamental_data()
{
    fund_data_.number_of_fields      = opt_spec_->field_of_view()->field_count();
    fund_data_.number_of_wavelengths = opt_spec_->spectral_region()->wvl_count();
    fund_data_.reference_wvl_index   = opt_spec_->spectral_region()->reference_index();
    fund_data_.reference_wvl_value   = opt_spec_->spectral_region()->reference_wvl();

    fund_data_.enp_distance = parax_data_->entrance_pupil_distance();
    fund_data_.enp_radius   = parax_data_->entrance_pupil_radius();
    fund_data_.exp_distance = parax_data_->exit_pupil_distance();
    fund_data_.exp_radius   = parax_data_->exit_pupil_radius();

    fund_data_.number_of_surfaces  = opt_assembly_->surface_count();
    fund_data_.image_surface_index = opt_assembly_->image_index();
    fund_data_.object_distance     = opt_assembly_->gap(0)->thi();
    fund_data_.image_distance      = opt_assembly_->image_space_gap()->thi();
}

void OpticalSystem::update_optical_spec()
{
    // update object coords
    Eigen::Vector3d obj_pt;

    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    const int field_type = opt_spec_->field_of_view()->field_type();

    for(int fi = 0; fi < fund_data_.number_of_fields; fi++){
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
            //obj_pt = -dir_tan*(fod_.obj_dist + fod_.enp_dist);
            obj_pt = -dir_tan*(parax_data_->object_distance() + parax_data_->entrance_pupil_distance());
            break;

        case FieldType::OBJ_HT:
            obj_pt(0) = fld_x;
            obj_pt(1) = fld_y;
            obj_pt(2) = 0.0;
            break;

        case FieldType::IMG_HT:
            img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
            //obj_pt = fod_.red*img_pt;
            obj_pt = parax_data_->reduction_rate()*img_pt;
            break;

        default:
            obj_pt = Eigen::Vector3d::Zero(3);
        }

        fld->set_object_coord(obj_pt);
    }

    // update aim pt
    if(opt_assembly_->surface_count() > 2)
    {
        SequentialTrace *tracer = new SequentialTrace(this);
        tracer->set_apply_vig(true);
        tracer->set_aperture_check(false);

        for(int fi = 0; fi < fund_data_.number_of_fields; fi++){
            Field* fld = opt_spec_->field_of_view()->field(fi);
            try{
                auto aim_pt = tracer->aim_chief_ray(fld, fund_data_.reference_wvl_value);
                opt_spec_->field_of_view()->field(fi)->set_aim_pt(aim_pt);
            }catch(TraceRayAimingFailedError &e){
                std::cerr << "Ray aiming failed at field " << fi << std::endl;
                continue;
            }
        }

        delete tracer;
    }
}


void OpticalSystem::update_paraxial_data()
{
    parax_data_->update(this);
}

void OpticalSystem::update_semi_diameters()
{
    SequentialTrace *tracer = new SequentialTrace(this);

    const int num_srf = opt_assembly_->surface_count();
    constexpr int num_ref_rays = 5;

    // initialize all surface
    for(int si = 0; si < num_srf; si++) {
        opt_assembly_->surface(si)->set_semi_diameter(0.0);
    }

    // update semi diameter
    std::vector< std::shared_ptr<Ray> > ref_rays;
    std::shared_ptr<Ray> chief_ray, mer_upper_ray, mer_lower_ray, sag_upper_ray, sag_lower_ray;


    for(int fi = 0; fi < fund_data_.number_of_fields; fi++)
    {
        Field* fld = opt_spec_->field_of_view()->field(fi);

        try{
            tracer->trace_reference_rays(ref_rays, fld, fund_data_.reference_wvl_value);
        }catch(std::out_of_range &e){
            std::cout << e.what() << std::endl;
            continue;
        }catch(...){
            std::cout << "undefined error in OpticalSystem::update_semidiameter" << std::endl;
        }

        std::vector<double> ray_size_list;
        for(int ri = 0; ri < num_ref_rays; ri++){
            ray_size_list.push_back(ref_rays[ri]->size());
        }
        int ray_size = *std::min_element(ray_size_list.begin(), ray_size_list.end());

        for(int si = 0; si < ray_size; si++) {

            std::vector<double> ray_ht_list({0.0, 0.0, 0.0, 0.0, 0.0});

            try{
                for(int ri = 0; ri < num_ref_rays; ri++){
                    ray_ht_list[ri] = ref_rays[ri]->at(si)->height();
                }
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

            double ray_ht_for_cur_fld = *std::max_element(ray_ht_list.begin(), ray_ht_list.end());

            double current_sd = opt_assembly_->surface(si)->semi_diameter();

            if(current_sd < ray_ht_for_cur_fld) {
                opt_assembly_->surface(si)->set_semi_diameter(ray_ht_for_cur_fld);
            }

        }

    }

    delete tracer;
}


void OpticalSystem::set_vignetting_factors()
{
    SequentialTrace *tracer = new SequentialTrace(this);

    for(int fi = 0; fi < fund_data_.number_of_fields; fi++){
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

    this->update_model();

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
