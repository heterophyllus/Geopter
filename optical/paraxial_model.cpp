#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "paraxial_model.h"
#include "optical_spec.h"
#include "wvl_spec.h"
#include "field_spec.h"
#include "pupil_spec.h"
#include "optical_model.h"
#include "sequential_model.h"
#include "surface.h"
#include "surface_profile.h"
#include "gap.h"
#include "medium.h"
#include "glass.h"



using namespace geopter;

void FirstOrderData::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void FirstOrderData::print(std::ostringstream& oss)
{
    const int fixed_w = 20;
    const int pre = 4;

    oss << std::setw(fixed_w) << std::left << "efl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl << std::endl;

    oss << std::setw(fixed_w) << std::left << "ffl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl << std::endl;

    oss << std::setw(fixed_w) << std::left << "pp1";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << pp1 << std::endl;

    oss << std::setw(fixed_w) << std::left << "ppk";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ppk << std::endl;

    oss << std::setw(fixed_w) << std::left << "bfl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl << std::endl;

    oss << std::setw(fixed_w) << std::left << "f/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

    oss << std::setw(fixed_w) << std::left << "red";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_ang";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "na obj";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_na << std::endl;

    oss << std::setw(fixed_w) << std::left << "na img";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_na << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_ht";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "opt inv";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv << std::endl;

    oss << std::ends;
}

ParaxialModel::ParaxialModel(OpticalModel* opt_model) :
    opt_model_(opt_model),
    opt_inv_(1.0)
{

}

ParaxialModel::~ParaxialModel()
{
    ax_.clear();
    pr_.clear();
    opt_model_ = nullptr;
}


ParaxialRay ParaxialModel::ax() const
{
    return ax_;
}

ParaxialRay ParaxialModel::pr() const
{
    return pr_;
}

void ParaxialModel::update_model()
{
    int stop = opt_model_->seq_model()->stop_surface();
    double wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    compute_first_order(stop, wvl);
    build_lens();
}

FirstOrderData ParaxialModel::first_order_data() const
{
    return fod_;
}


void ParaxialModel::build_lens()
{
    /*
    auto _seq_model = _opt_model->seq_model();

    _opt_inv = _fod.opt_inv;

    //_ax.clear();
    //_pr.clear();

    auto path = _seq_model->path();
    int num_srfs = (int)path.size();

    auto ax_ray = _ax;
    auto pr_ray = _pr;
    _ax.clear();
    _pr.clear();

    for(int i = 0; i < num_srfs; i++)
    {
        double n;
        ParaxialRayAtSurface ray_cmp;

        ray_cmp.ht = ax_ray.at(i).ht;
        ray_cmp.slp = n*ax_ray.at(i).slp;
        ray_cmp.aoi = n*ax_ray.at(i).aoi;
        _ax.append(ray_cmp);

        ray_cmp.ht = pr_ray.at(i).ht;
        ray_cmp.slp = n*pr_ray.at(i).slp;
        ray_cmp.aoi = n*pr_ray.at(i).aoi;
        _pr.append(ray_cmp);

    }
    */

    /*
    LensData sys = seq_path_to_paraxial_lens(_seq_model->path());
    _sys = sys;

    auto ax_ray = _parax_data.ax_ray;
    auto pr_ray = _parax_data.pr_ray;
    auto fod    = _parax_data.fod;

    _opt_inv = _fod.opt_inv;

    _ax.clear();
    _pr.clear();

    for(int i = 0; i < (int)sys.size(); i++){
        auto n = sys[i].indx;
        ParaxialRayComponent ray_cmp;
        ray_cmp.ht = ax_ray[i].ht;
        ray_cmp.slp = n*ax_ray[i].slp;
        ray_cmp.aoi = n*ax_ray[i].aoi;
        _ax.push_back(ray_cmp);

        ray_cmp.ht = pr_ray[i].ht;
        ray_cmp.slp = n*pr_ray[i].slp;
        ray_cmp.aoi = n*pr_ray[i].aoi;
        _pr.push_back(ray_cmp);
    }
    */

}

void ParaxialModel::compute_first_order(int stop, double wvl)
{
    auto seq_model = opt_model_->seq_model();
    int start = 1;
    double n_0 = seq_model->z_dir(start-1) * seq_model->central_rndx(start-1);
    double uq0 = 1.0/n_0;

    ParaxialRayHtAndSlp start_yu, start_yu_bar;
    start_yu.ht = 1.0;
    start_yu.slp = 0.0;
    start_yu_bar.ht=0.0;
    start_yu_bar.slp = uq0;
    //ParaxialRayPair p_q_ray = paraxial_trace(seq_model->path(wvl),start,start_yu,start_yu_bar);
    //std::vector<ParaxialRayComponent> p_ray = p_q_ray.p_ray;
    //std::vector<ParaxialRayComponent> q_ray = p_q_ray.p_ray_bar;
    ParaxialRay p_ray, q_ray;
    paraxial_trace(p_ray, q_ray, start, start_yu, start_yu_bar);


    auto n_k = seq_model->z_dir(-1)*seq_model->central_rndx(-1);
    int img;
    if(seq_model->surface_count() > 2){
        img = -2;
        img = p_ray.size() -2 ;
    }else{
        img = -1;
        img = p_ray.size() -1;
    }

    /*
    auto ak1 = p_ray[img].ht;
    auto bk1 = q_ray[img].ht;
    auto ck1 = n_k*p_ray[img].slp;
    auto dk1 = n_k*q_ray[img].slp;
    */
    double ak1 = p_ray.at(img).ht;
    double bk1 = q_ray.at(img).ht;
    double ck1 = n_k*p_ray.at(img).slp;
    double dk1 = n_k*q_ray.at(img).slp;

    // The code below computes the object yu and yu_bar values
    int orig_stop = stop;

    /*
    std::vector<ParaxialRayComponent> ax;
    std::vector<ParaxialRayComponent> pr;
    ParaxialRayHtAndSlp yu;
    ParaxialRayHtAndSlp yu_bar;
    */
    ParaxialRay ax, pr;
    ParaxialRayHtAndSlp yu, yu_bar;

    if(stop == 0)
    {
        if(opt_model_->paraxial_model()->ax().size() > 0)
        {
            /*
            ax = opt_model->paraxial_model()->ax();
            pr = opt_model->paraxial_model()->pr();
            */
            ax = ax_;
            pr = pr_;
            yu.ht = 0.0;
            yu.slp = ax.at(0).slp/n_0;
            yu_bar.ht = pr.at(0).ht;
            yu_bar.slp = pr.at(0).slp/n_0;
        }
        else{
            // temporarily set stop to surface 1
            stop = 1;
        }
    }

    if(stop > 0){
        double n_s = seq_model->z_dir(stop) * seq_model->central_rndx(stop);
        /*
        double as1 = p_ray[stop].ht;
        double bs1 = q_ray[stop].ht;
        double cs1 = n_s*p_ray[stop].slp;
        double ds1 = n_s*q_ray[stop].slp;
        */
        double as1 = p_ray.at(stop).ht;
        double bs1 = q_ray.at(stop).ht;
        double cs1 = n_s*p_ray.at(stop).slp;
        double ds1 = n_s*q_ray.at(stop).slp;

        // find entrance pupil location w.r.t. first surface
        double ybar1 = -bs1;
        double ubar1 = as1;
        double n_0 = seq_model->gap(0)->medium()->rindex(wvl);
        double enp_dist = -ybar1/(n_0*ubar1);

        double thi0 = seq_model->gap(0)->thi();

        // calculate reduction ratio for given object distance
        double red = dk1 + thi0*ck1;
        double obj2enp_dist = thi0 + enp_dist;

        yu.ht  = 1.0;
        yu.slp = 0.0;

        auto pupil = opt_model_->optical_spec()->pupil_spec();
        double slp0 = 0.0;
        double slpk;

        switch (pupil->pupil_type()) {
        case PupilType::EPD:
            slp0 = 0.5*pupil->value()/obj2enp_dist;
            break;
        case PupilType::NAO:
            slp0 = n_0*tan(asin(pupil->value()/n_0));
            break;
        case PupilType::FNO:
            slpk = -1.0/(2.0*pupil->value());
            slp0 = slpk/red;
            break;
        case PupilType::NA:
            slpk = n_k*tan(asin(pupil->value()/n_k));
            slp0 = slpk/red;
            break;
        default:
            slp0 = 0.0;
        }

        /*
        auto pupil_keys = pupil->pupil_keys();

        if(pupil_keys.obj_img_key == "object")
        {
            if(pupil_keys.value_key == "pupil"){
                slp0 = 0.5*pupil->value()/obj2enp_dist;
            }
            else if(pupil_keys.value_key == "NA"){
                slp0 = n_0*tan(asin(pupil->value()/n_0));
            }
        }
        else if(pupil_keys.obj_img_key == "image"){
            if(pupil_keys.value_key == "f/#"){
                double slpk = -1.0/(2.0*pupil->value());
                slp0 = slpk/red;
            }
            else if(pupil_keys.value_key == "NA"){
                double slpk = n_k*tan(asin(pupil->value()/n_k));
                slp0 = slpk/red;
            }
        }
        */

        yu.ht = 0.0;
        yu.slp = slp0;

        yu_bar.ht  = 1.0;
        yu_bar.slp = 0.0;

        auto fov = opt_model_->optical_spec()->field_of_view();
        //auto field_keys = fov->key();
        double max_fld = fov->max_field();
        double ybar0 = 0.0;
        double slpbar0 = 0.0;
        if(max_fld == 0.0){
            max_fld = 1.0;
        }

        double ang;

#ifdef _MSC_VER
        double tmp_pi = 4.0*atan(1.0);

        switch (fov->field_type()) {
        case FieldType::OBJ_ANG:
            ang = max_fld* tmp_pi/180.0; // degree to radian
            slpbar0 = tan(ang);
            ybar0 = -slpbar0*obj2enp_dist;
            break;
        case FieldType::OBJ_HT:
            ybar0 = -max_fld;
            slpbar0 = -ybar0/obj2enp_dist;
            break;
        case FieldType::IMG_HT:
            ybar0 = red*max_fld;
            slpbar0 = -ybar0/obj2enp_dist;
            break;
        }
#else
        switch (fov->field_type()) {
        case FieldType::OBJ_ANG:
            ang = max_fld* M_PI/180.0; // degree to radian
            slpbar0 = tan(ang);
            ybar0 = -slpbar0*obj2enp_dist;
            break;
        case FieldType::OBJ_HT:
            ybar0 = -max_fld;
            slpbar0 = -ybar0/obj2enp_dist;
            break;
        case FieldType::IMG_HT:
            ybar0 = red*max_fld;
            slpbar0 = -ybar0/obj2enp_dist;
            break;
        }
#endif



        yu_bar.ht  = ybar0;
        yu_bar.slp = slpbar0;
    }

    stop = orig_stop;

    // We have the starting coordinates, now trace the rays
    /*
    auto ax_pr_ray = paraxial_trace(seq_model->path(wvl), 0, yu, yu_bar);
    auto ax_ray = ax_pr_ray.p_ray;
    auto pr_ray = ax_pr_ray.p_ray_bar;
    */
    ParaxialRay ax_ray, pr_ray;
    paraxial_trace(ax_ray, pr_ray, 0, yu, yu_bar);

    // Calculate the optical invariant
    n_0 = seq_model->central_rndx(0);
    double opt_inv = n_0*( ax_ray.at(1).ht*pr_ray.at(0).slp - pr_ray.at(1).ht*ax_ray.at(0).slp );

    // Fill in the contents of the FirstOrderData struct
    FirstOrderData fod;
    double obj_dist = seq_model->gap(0)->thi();
    double img_dist = seq_model->gap(-1)->thi();

    fod.opt_inv  = opt_inv;
    fod.obj_dist = obj_dist;
    fod.img_dist = img_dist;

    if(ck1 == 0.0){
        fod.power = 0.0;
        fod.efl   = 0.0;
        fod.pp1   = 0.0;
        fod.ppk   = 0.0;
    }
    else{
        fod.power = -ck1;
        fod.efl   = -1.0/ck1;
        fod.pp1   = (dk1-1.0)*(n_0/ck1);
        fod.ppk   = (p_ray.at(p_ray.size()-2).ht - 1.0)*(n_k/ck1);
    }
    fod.ffl = fod.pp1 - fod.efl;
    fod.bfl = fod.efl - fod.ppk;
    fod.fno = -1.0/(2.0*n_k*ax_ray.at(ax_ray.size()-1).slp);
    fod.m = ak1 + ck1*img_dist/n_k;
    fod.red = dk1 + ck1*obj_dist;
    fod.n_obj = n_0;
    fod.n_img = n_k;
    fod.img_ht = -fod.opt_inv/(n_k*ax_ray.at(ax_ray.size()-1).slp);
    fod.obj_ang = atan(pr_ray.at(0).slp) *180/M_PI;

    if(pr_ray.at(0).slp != 0){
        double nu_pr0 = n_0*pr_ray.at(0).slp;
        fod.enp_dist = -pr_ray.at(1).ht/nu_pr0;
        fod.enp_radius = abs(fod.opt_inv/nu_pr0);
    }
    else{
        fod.enp_dist = -1e10;
        fod.enp_radius = 1e10;
    }

    if(pr_ray.at(pr_ray.size()-1).slp != 0){
        fod.exp_dist = -(pr_ray.at(pr_ray.size()-1).ht/pr_ray.at(pr_ray.size()-1).slp - fod.img_dist);
        fod.exp_radius = abs(fod.opt_inv/(n_k*pr_ray.at(pr_ray.size()-1).slp));
    }
    else{
        fod.exp_dist = -1e10;
        fod.exp_radius = 1e10;
    }

    // compute object and image space numerical apertures
    fod.obj_na = n_0*sin(atan(seq_model->z_dir(0)*ax_ray.at(0).slp));
    //fod.img_na = n_k*sin(atan(seq_model->z_dir(-1)*ax_ray[ax_ray.size()-1].slp));
    fod.img_na = n_k*sin(atan(seq_model->z_dir(-1)*ax_ray.back().slp));

    /*
    ParaxialData pd;
    pd.ax_ray = ax_ray;
    pd.pr_ray = pr_ray;
    pd.fod    = fod;

    return pd;
    */

    ax_ = ax_ray;
    pr_ = pr_ray;

    fod_ = fod;
}


void ParaxialModel::paraxial_trace(ParaxialRay& p_ray, ParaxialRay& p_ray_bar, int start, ParaxialRayHtAndSlp start_yu, ParaxialRayHtAndSlp start_yu_bar)
{
    /*
    std::vector<ParaxialRayComponent> p_ray;
    std::vector<ParaxialRayComponent> p_ray_bar;
    */

    p_ray.clear();
    p_ray_bar.clear();


    double wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    Path path = opt_model_->seq_model()->path(wvl);

    int path_itr_cnt = 0;
    auto path_itr = path.begin();
    auto before = *path_itr;
    auto z_dir_before = before.z_dir;
    double n_before;
    if(z_dir_before > 0.0){
        n_before = before.rndx;
    }else{
        n_before = -before.rndx;
    }

    /*
    ParaxialRayComponent b4_yui;
    b4_yui.ht  = start_yu.ht;
    b4_yui.slp = start_yu.slp;
    */

    ParaxialRayAtSurface b4_yui;
    b4_yui.ht  = start_yu.ht;
    b4_yui.slp = start_yu.slp;

    /*
    ParaxialRayComponent b4_yui_bar;
    b4_yui_bar.ht = start_yu_bar.ht;
    b4_yui_bar.slp = start_yu_bar.slp;
    */

    ParaxialRayAtSurface b4_yui_bar;
    b4_yui_bar.ht = start_yu_bar.ht;
    b4_yui_bar.slp = start_yu_bar.slp;

    if(start == 1)
    {
        // compute object coords from 1st surface data
        auto t0 = before.gap->thi();
        auto obj_ht = start_yu.ht - t0*start_yu.slp;
        auto obj_htb = start_yu_bar.ht - t0*start_yu_bar.slp;

        b4_yui.ht  = obj_ht;
        b4_yui.slp = start_yu.slp;

        b4_yui_bar.ht  = obj_htb;
        b4_yui_bar.slp = start_yu_bar.slp;
    }

    double cv = before.srf->profile()->cv();

    // calculate angle of incidence (aoi)
    double aoi     = b4_yui.slp + b4_yui.ht*cv;
    double aoi_bar = b4_yui_bar.slp + b4_yui_bar.ht*cv;

    b4_yui.aoi = aoi;
    b4_yui_bar.aoi = aoi_bar;

    /*
    p_ray.push_back(b4_yui);
    p_ray_bar.push_back(b4_yui_bar);
    */
    p_ray.append(b4_yui);
    p_ray_bar.append(b4_yui_bar);

    while(true)
    {
        ++path_itr;
        ++path_itr_cnt;

        try {
            auto after = *path_itr;

            double z_dir_after = after.z_dir;
            double n_after;
            if(z_dir_after > 0){
                n_after = after.rndx;
            }else{
                n_after = -after.rndx;
            }

            // Transfer
            double t = before.gap->thi();
            double cur_ht = b4_yui.ht + t*b4_yui.slp;
            double cur_htb = b4_yui_bar.ht + t*b4_yui_bar.slp;

            // Refraction/Reflection
            auto srf = after.srf;
            double k = n_before/n_after;

            // calculate slope after refraction/reflection
            double pwr = srf->optical_power();
            double cur_slp = k*b4_yui.slp - cur_ht*pwr/n_after;
            double cur_slpb = k*b4_yui_bar.slp - cur_htb*pwr/n_after;

            // calculate angle of incidence (aoi)
            cv = srf->profile()->cv();
            aoi = cur_slp + cur_ht*cv;
            aoi_bar = cur_slpb + cur_htb*cv;

            //ParaxialRayComponent yu;
            ParaxialRayAtSurface yu;
            yu.ht = cur_ht;
            yu.slp = cur_slp;
            yu.aoi = aoi;

            //ParaxialRayComponent yu_bar;
            ParaxialRayAtSurface yu_bar;
            yu_bar.ht = cur_htb;
            yu_bar.slp = cur_slpb;
            yu_bar.aoi = aoi_bar;

            /*
            p_ray.push_back(yu);
            p_ray_bar.push_back(yu_bar);
            */
            p_ray.append(yu);
            p_ray_bar.append(yu_bar);

            b4_yui = yu;
            b4_yui_bar = yu_bar;

            n_before = n_after;
            z_dir_before = z_dir_after;
            before = after;


            if(path_itr_cnt == (int)path.size()-1){
                break;
            }

        }  catch (...) {
            break;
        }
    }

    /*
    ParaxialRayPair par_ray_pair;
    par_ray_pair.p_ray = p_ray;
    par_ray_pair.p_ray_bar = p_ray_bar;

    return par_ray_pair;
    */


}


