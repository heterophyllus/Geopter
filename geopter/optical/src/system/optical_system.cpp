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

#include "system/optical_system.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include "Eigen/Dense"
#include "nlohmann/json.hpp"

#include "paraxial/paraxial_trace.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"


using namespace geopter;


OpticalSystem::OpticalSystem() :
    title_(""),
    note_("")
{
    opt_spec_     = std::make_unique<OpticalSpec>(this);
    opt_assembly_ = std::make_unique<OpticalAssembly>(this);
    material_lib_ = std::make_unique<MaterialLibrary>();
    fod_   = std::make_unique<FirstOrderData>(this);
}

OpticalSystem::~OpticalSystem()
{
    opt_assembly_.reset();
    opt_spec_.reset();
    material_lib_.reset();
    fod_.reset();
}

void OpticalSystem::clear()
{
    title_ = "";
    note_ = "";
    opt_assembly_->clear();
    opt_spec_->clear();
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
    // be carefull to the updating order
    opt_assembly_->update_transforms();
    opt_assembly_->update_solve();
    fod_->update();
    opt_spec_->update();
    opt_assembly_->update_semi_diameters();
}


void OpticalSystem::set_vignetting_factors()
{
    SequentialTrace *tracer = new SequentialTrace(this);
    const int num_flds = FieldSpec::number_of_fields();

    for(int fi = 0; fi < num_flds; fi++){
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

    const int num_wvls = opt_spec_->spectral_region()->number_of_wavelengths();

    for(int wi = 0; wi < num_wvls; wi++){
        auto wvl = opt_spec_->spectral_region()->wavelength(wi);

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
        std::string surface_type  = s->profile_name();
        double      cv            = s->cv();


        json_data["Assembly"][cur_idx]["Label"]     = surface_label;
        json_data["Assembly"][cur_idx]["Type"]      = surface_type;
        json_data["Assembly"][cur_idx]["Curvature"] = cv;

        if(surface_type == "SPH"){
            // do nothing
        }else if(surface_type == "ASP"){
            EvenPolynomial* prf = s->profile<EvenPolynomial>();
            json_data["Assembly"][cur_idx]["Conic"] = prf->conic();
            for(int ci = 0; ci < prf->coef_count(); ci++){
                json_data["Assembly"][cur_idx]["Coefs"].push_back( prf->coef(ci) );
            }

        }else if(surface_type == "ODD"){
            OddPolynomial *prf = s->profile<OddPolynomial>();
            if(prf){
                json_data["Assembly"][cur_idx]["Conic"] = prf->conic();
                for(int ci = 0; ci < prf->coef_count(); ci++){
                    json_data["Assembly"][cur_idx]["Coefs"].push_back( prf->coef(ci) );
                }
            }
        }

        if( ! s->is_aperture<NoneAperture>()){
            std::string aperture_type = s->aperture_shape();
            json_data["Assembly"][cur_idx]["Aperture"]["Type"]  = aperture_type;

            if(aperture_type == "Circular"){
                double cir_ap_r = s->clear_aperture<Circular>()->radius();
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
    std::ofstream fout(filepath, std::ios::out);
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
            opt_spec_->spectral_region()->add_wavelength(wvl_val[wi], wvl_wt[wi],color);
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

        //opt_assembly_->add_surface_and_gap();
        opt_assembly_->insert(-1);

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
            srf->set_profile<Spherical>(cv);
        }
        else if( surf_type == "ASP" ) {
            double conic = json_data["Assembly"][cur_idx]["Conic"].get<double>();
            std::vector<double> coefs = json_data["Assembly"][cur_idx]["Coefs"].get< std::vector<double> >();

            srf->set_profile<EvenPolynomial>(cv, conic, coefs);
        }
        else if(surf_type == "ODD") {
            double conic = json_data["Assembly"][cur_idx]["Conic"].get<double>();
            std::vector<double> coefs = json_data["Assembly"][cur_idx]["Coefs"].get< std::vector<double> >();

            srf->set_profile<OddPolynomial>(cv, conic, coefs);
        }


        // aperture
        if( json_data["Assembly"][cur_idx].contains("Aperture") ){
            std::string aperture_type = json_data["Assembly"][cur_idx]["Aperture"]["Type"].get<std::string>();
            if(aperture_type == "Circular"){
                double cir_ap_r = json_data["Assembly"][cur_idx]["Aperture"]["Radius"].get<double>();
                srf->set_clear_aperture<Circular>(cir_ap_r, cir_ap_r);
            }
        }

        // gap
        double thi = json_data["Assembly"][cur_idx]["Thickness"].get<double>();
        std::string mat_name = json_data["Assembly"][cur_idx]["Material"].get<std::string>();
        auto mat = material_lib_->find(mat_name);

        auto gap = opt_assembly_->current_gap();
        gap->set_thi(thi);
        gap->set_material(mat);

        ci++;
    }

    int stop_idx = json_data["Assembly"]["Stop"].get<int>();
    opt_assembly_->set_stop(stop_idx);

    // <--- Assembly end

    this->update_model();

}

void OpticalSystem::print(std::ostringstream &oss)
{
    oss << "Title: " << title_ << std::endl;
    oss << std::endl;
    oss << "Note: " << std::endl;
    oss << note_ << std::endl;
    oss << std::endl;

    oss << "SPECIFICATIONS..." << std::endl;
    oss << std::endl;
    opt_spec_->print(oss);

    oss << "FIRST ORDER DATA..." << std::endl;
    fod_->print(oss);
    oss << std::endl;

    oss << "LENS DATA..." << std::endl;
    oss << std::endl;
    opt_assembly_->print(oss);
    oss << std::endl;

}
