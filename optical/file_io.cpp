#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include "nlohmann/json.hpp"

#include "file_io.h"

#include "optical_model.h"
#include "optical_spec.h"
#include "pupil_spec.h"
#include "wvl_spec.h"
#include "field_spec.h"
#include "sequential_model.h"
#include "surface.h"
#include "gap.h"
#include "surface_profile.h"
#include "spherical.h"
#include "even_polynomial.h"
#include "aperture.h"
#include "circular.h"



using namespace geopter;

FileIO::FileIO()
{

}

bool FileIO::save_to_json(const OpticalModel& opt_model, std::string json_path)
{
    nlohmann::json json_data;

    json_data["title"] = opt_model.name();


    /* PupilSpec */
    auto pupil = opt_model.optical_spec()->pupil_spec();
    json_data["OpticalSpec"]["PupilSpec"]["type"]  = pupil->pupil_type();
    json_data["OpticalSpec"]["PupilSpec"]["value"] = pupil->value();


    /* FieldSpec */
    int field_type = opt_model.optical_spec()->field_of_view()->field_type();
    json_data["OpticalSpec"]["FieldSpec"]["FieldType"] = field_type;

    int num_flds = opt_model.optical_spec()->field_of_view()->field_count();
    json_data["OpticalSpec"]["FieldSpec"]["NumFields"] = num_flds;

    for(int fi = 0; fi < num_flds; fi++){

        std::string fld_idx = "F" + std::to_string(fi);

        auto fld = opt_model.optical_spec()->field_of_view()->field(fi);

        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["x"]     = fld->x();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["y"]     = fld->y();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["wt"]    = fld->weight();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vux"]   = fld->vux();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vlx"]   = fld->vlx();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vuy"]   = fld->vuy();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vly"]   = fld->vly();
        json_data["OpticalSpec"]["FieldSpec"][fld_idx]["color"] = { fld->render_color().r, fld->render_color().g, fld->render_color().b, fld->render_color().a };
    }

    /* WvlSpec */
    int ref_wvl_idx = opt_model.optical_spec()->spectral_region()->reference_index();
    json_data["OpticalSpec"]["WvlSpec"]["ReferenceIndex"] = ref_wvl_idx;

    int num_wvls = opt_model.optical_spec()->spectral_region()->wvl_count();
    json_data["OpticalSpec"]["WvlSpec"]["NumWvls"] = num_wvls;

    for(int wi = 0; wi < num_wvls; wi++){

        std::string wvl_idx = "W" + std::to_string(wi);

        auto wvl = opt_model.optical_spec()->spectral_region()->wvl(wi);
        json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["value"]  = wvl->value();
        json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["weight"] = wvl->weight();
        json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["color"]  = { wvl->render_color().r, wvl->render_color().g, wvl->render_color().b, wvl->render_color().a };
    }

    /* surface data */
    int num_srfs = opt_model.seq_model()->surface_count();
    json_data["Surfaces"]["NumSurfaces"] = num_srfs;

    int stop_surface = opt_model.seq_model()->stop_surface();
    json_data["Surfaces"]["Stop"] = stop_surface;

    int si = 0;
    int gi = 0;

    for( si = 0; si < num_srfs-1; si++){ //sO..sI-1

        std::string srf_idx = "S" + std::to_string(si);

        gi = si;
        auto s = opt_model.seq_model()->surface(si);
        auto g = opt_model.seq_model()->gap(gi);

        std::string surface_label = s->label();
        std::string surface_type  = s->profile()->name();
        double      cv            = s->profile()->cv();
        double      thi           = g->thi();
        std::string medium_name   = g->medium()->display_name();
        std::string aperture_type = s->aperture_shape();

        json_data["Surfaces"][srf_idx]["Label"]     = surface_label;
        json_data["Surfaces"][srf_idx]["Type"]      = surface_type;
        json_data["Surfaces"][srf_idx]["Curvature"] = cv;
        json_data["Surfaces"][srf_idx]["Thickness"] = thi;
        json_data["Surfaces"][srf_idx]["Medium"]    = medium_name;
        json_data["Surfaces"][srf_idx]["Aperture"]["Type"]  = aperture_type;

        if(surface_type == "ASP"){
            EvenPolynomial *prf = dynamic_cast<EvenPolynomial*>(s->profile());
            if(prf){
                for(int ci = 0; ci < prf->coef_count(); ci++){
                    json_data["Surfaces"][srf_idx]["Coef" + std::to_string(ci)] = prf->coef(ci);
                }
            }
        }

        if(aperture_type == "None"){
            continue;
        }else if(aperture_type == "Circular"){
            double cir_ap_r = dynamic_cast<Circular*>(s->clear_aperture())->max_dimension();
            json_data["Surfaces"][srf_idx]["Aperture"]["Radius"] = cir_ap_r;
        }
    }

    // image surface
    si = num_srfs-1;
    {
        std::string srf_idx = "S" + std::to_string(si);
        auto s = opt_model.seq_model()->surface(si);
        double cv = s->profile()->cv();

        json_data["Surfaces"][srf_idx]["Curvature"] = cv;
    }


    /* output to file */
    std::ofstream fout(json_path);
    fout << std::setw(4) << json_data << std::endl;

    return true;
}


bool FileIO::load_from_json(OpticalModel& opt_model, std::string json_path)
{
    std::ifstream ifs(json_path);
    if(!ifs){
        std::cout << "File Open Error: " << json_path << std::endl;
        return false;
    }

    nlohmann::json json_data;
    ifs >> json_data;


    /* OpticalSpec */
    if(json_data.find("OpticalSpec") != json_data.end())
    {
        /* Pupil Spec */
        int    pupil_type  = json_data["OpticalSpec"]["PupilSpec"]["type"].get<int>();
        double pupil_value = json_data["OpticalSpec"]["PupilSpec"]["value"].get<double>();
        opt_model.optical_spec()->pupil_spec()->set_pupil_type(pupil_type);
        opt_model.optical_spec()->pupil_spec()->set_value(pupil_value);

        /* Field Spec */
        int field_type = 0;
        try{
            field_type = json_data["OpticalSpec"]["FieldSpec"]["FieldType"].get<int>();
        }catch(...){
            field_type = 0;
        }
        opt_model.optical_spec()->field_of_view()->set_field_type(field_type);

        int num_flds = json_data["OpticalSpec"]["FieldSpec"]["NumFields"].get<int>();
        opt_model.optical_spec()->field_of_view()->clear();
        for(int fi = 0; fi < num_flds; fi++)
        {
            std::string fld_idx = "F" + std::to_string(fi);

            double x  = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["x"].get<double>();
            double y  = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["y"].get<double>();
            double wt  = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["wt"].get<double>();
            double vlx = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vlx"].get<double>();
            double vly = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vly"].get<double>();
            double vux = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vux"].get<double>();
            double vuy = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["vuy"].get<double>();

            double r = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["color"][0].get<double>();
            double g = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["color"][1].get<double>();
            double b = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["color"][2].get<double>();
            double a = json_data["OpticalSpec"]["FieldSpec"][fld_idx]["color"][3].get<double>();

            opt_model.optical_spec()->field_of_view()->add_field(x,y,wt,Rgb(r,g,b,a),vuy,vly,vux,vlx);
        }

        /* Wvl Spec */
        opt_model.optical_spec()->spectral_region()->clear();

        int ref_wvl_idx = 0;
        try{
            ref_wvl_idx = json_data["OpticalSpec"]["WvlSpec"]["ReferenceIndex"].get<int>();
        }catch(...){
            ref_wvl_idx = 0;
        }
        opt_model.optical_spec()->spectral_region()->set_reference_index(ref_wvl_idx);

        int num_wvls = json_data["OpticalSpec"]["WvlSpec"]["NumWvls"].get<int>();
        for(int wi = 0; wi < num_wvls; wi++)
        {
            std::string wvl_idx = "W" + std::to_string(wi);

            double value = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["value"].get<double>();
            double wt    = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["weight"].get<double>();
            double r     = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["color"][0].get<double>();
            double g     = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["color"][1].get<double>();
            double b     = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["color"][2].get<double>();
            double a     = json_data["OpticalSpec"]["WvlSpec"][wvl_idx]["color"][3].get<double>();

            opt_model.optical_spec()->spectral_region()->add(value, wt, Rgb(r,g,b,a));
        }

    }else{
        std::cout << "Not Found:" << "OpticalSpec" << std::endl;
        return false;
    }


    /* Surface Data */
    if(json_data.find("Surfaces") != json_data.end())
    {
        int num_srfs = json_data["Surfaces"]["NumSurfaces"].get<int>();

        opt_model.seq_model()->clear();
        opt_model.seq_model()->initialize_arrays();

        // Object and Image have been already added to the system as default
        for(int si = 1; si < num_srfs-1; si++)
        {
            std::string srf_idx = "S" + std::to_string(si);

            std::string surface_type  = json_data["Surfaces"][srf_idx]["Type"].get<std::string>();
            double      cv            = json_data["Surfaces"][srf_idx]["Curvature"].get<double>();
            double      thi           = json_data["Surfaces"][srf_idx]["Thickness"].get<double>();
            std::string medium_name   = json_data["Surfaces"][srf_idx]["Medium"].get<std::string>();

            if(surface_type == "SPH"){
                opt_model.seq_model()->add_surface(1.0/cv,thi,medium_name);
            }else if(surface_type == "ASP"){
                opt_model.seq_model()->add_surface(std::make_unique<EvenPolynomial>(cv),thi,medium_name);
                EvenPolynomial* prf = dynamic_cast<EvenPolynomial*>(opt_model.seq_model()->current_surface()->profile());
                for(int ci = 0 ; ci < 10; ci++){
                    double val = json_data["Surfaces"][srf_idx]["Coef" + std::to_string(ci)].get<double>();
                    prf->set_coef(ci, val);
                }
            }

            std::string surface_label = json_data["Surfaces"][srf_idx]["Label"].get<std::string>();
            opt_model.seq_model()->current_surface()->set_label(surface_label);

            try{
                std::string aperture_type = json_data["Surfaces"][srf_idx]["Aperture"]["Type"].get<std::string>();
                if(aperture_type == "None"){
                    continue;
                }else if(aperture_type == "Circular"){
                    double cir_ap_r = json_data["Surfaces"][srf_idx]["Aperture"]["Radius"].get<double>();
                    opt_model.seq_model()->surface(si)->set_clear_aperture(std::make_unique<Circular>(cir_ap_r));
                }
            }catch(...){
                continue;
            }



        }
         // obect space gap
        double thi0 = json_data["Surfaces"]["S0"]["Thickness"].get<double>();
        opt_model.seq_model()->gap(0)->set_thi(thi0);

        // set stop
        int stop_surface = json_data["Surfaces"]["Stop"].get<int>();
        opt_model.seq_model()->set_stop(stop_surface);

    }else{
        std::cout << "Not Found:" << "Surfaces" << std::endl;
        return false;
    }


    return true;
}


void FileIO::split_path(const std::string &full_path, std::string &folder_path, std::string &file_base, std::string &ext)
{
    file_base = std::filesystem::path(full_path).stem().u8string();
    folder_path = std::filesystem::path(full_path).parent_path().u8string();
    ext = std::filesystem::path(full_path).extension().u8string();
}

std::vector<std::string> FileIO::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::string item;
    for (auto ch: s) {
        if (ch == delim) {
            if (!item.empty()){
                elems.push_back(item);
            }
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty()){
        elems.push_back(item);
    }

    return elems;
}

bool FileIO::starts_with(const std::string &s, const std::string &prefix)
{
    auto size = prefix.size();
    if (s.size() < size) {
        return false;
    }
    return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

bool FileIO::contains(const std::string &s, const std::string &item)
{
    if(s.find(item) != std::string::npos){
        return true;
    }else{
        return false;
    }
}

double FileIO::round_n(double number, int n)
{
    double ret = number * pow(10,n-1);
    ret = round(ret);
    ret /= pow(10, n-1);

    return ret;
}

double FileIO::floor_n(double number, int n)
{
    double ret = number * pow(10,n-1);
    ret = floor(ret);
    ret /= pow(10, n-1);

    return ret;
}

double FileIO::ceil_n(double number, int n)
{
    double ret = number * pow(10,n-1);
    ret = ceil(ret);
    ret /= pow(10, n-1);

    return ret;
}
