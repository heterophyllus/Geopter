#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Eigen/LU"

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
    //int stop = opt_model_->seq_model()->stop_surface();
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    compute_paraxial_ray(ax_, pr_, ref_wvl);
    fod_ = compute_first_order();
}

FirstOrderData ParaxialModel::first_order_data() const
{
    return fod_;
}


FirstOrderData ParaxialModel::compute_first_order()
{
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    auto seq_model = opt_model_->seq_model();
    int start = 1;
    int stop = seq_model->stop_surface();
    double n_0 = seq_model->z_dir(start-1) * seq_model->central_rndx(start-1);
    double uq0 = 1.0/n_0;

    ParaxialRayHtAndSlp start_yu, start_yu_bar;
    start_yu.ht = 1.0;
    start_yu.slp = 0.0;
    start_yu_bar.ht = 0.0;
    start_yu_bar.slp = uq0;

    ParaxialRay p_ray, q_ray;
    paraxial_trace(p_ray, q_ray, start, start_yu, start_yu_bar, ref_wvl);


    auto n_k = seq_model->z_dir(-1)*seq_model->central_rndx(-1);
    int img;
    if(seq_model->surface_count() > 2){
        img = p_ray.size() -2 ;
    }else{
        img = p_ray.size() -1;
    }


    double ak1 = p_ray.at(img).ht;
    double bk1 = q_ray.at(img).ht;
    double ck1 = n_k*p_ray.at(img).slp;
    double dk1 = n_k*q_ray.at(img).slp;

    // The code below computes the object yu and yu_bar values
    int orig_stop = stop;


    ParaxialRay ax, pr;
    ParaxialRayHtAndSlp yu, yu_bar;

    if(stop == 0)
    {
        if(opt_model_->paraxial_model()->ax().size() > 0)
        {
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
        //double n_s = seq_model->z_dir(stop) * seq_model->central_rndx(stop);

        double as1 = p_ray.at(stop).ht;
        double bs1 = q_ray.at(stop).ht;
        //double cs1 = n_s*p_ray.at(stop).slp;
        //double ds1 = n_s*q_ray.at(stop).slp;

        // find entrance pupil location w.r.t. first surface
        double ybar1 = -bs1;
        double ubar1 = as1;
        double n_0 = seq_model->gap(0)->medium()->rindex(ref_wvl);
        double enp_dist = -ybar1/(n_0*ubar1);

        double thi0 = seq_model->gap(0)->thi();

        // calculate reduction ratio for given object distance
        double red;
        if(std::isinf(thi0)){
            red = 0.0;
        }else{
            red = dk1 + thi0*ck1;
        }
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



        yu.ht = 0.0;
        yu.slp = slp0;

        yu_bar.ht  = 1.0;
        yu_bar.slp = 0.0;

        auto fov = opt_model_->optical_spec()->field_of_view();
        double max_fld = fov->max_field();
        double ybar0 = 0.0;
        double slpbar0 = 0.0;
        if(max_fld == 0.0){
            max_fld = 1.0;
        }

        double ang;

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


        yu_bar.ht  = ybar0;
        yu_bar.slp = slpbar0;
    }

    //stop = orig_stop;

    // We have the starting coordinates, now trace the rays
    ParaxialRay ax_ray, pr_ray;
    paraxial_trace(ax_ray, pr_ray, 0, yu, yu_bar, ref_wvl);

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
    fod.img_na = n_k*sin(atan(seq_model->z_dir(-1)*ax_ray.last().slp));


    //ax_ = ax_ray;
    //pr_ = pr_ray;

    //fod_ = fod;
    return fod;
}



void ParaxialModel::compute_paraxial_ray(ParaxialRay& ax_ray, ParaxialRay& pr_ray, double wvl)
{
    /*
     *  Get entrance pupil diameter and distance
     *  Get start y and u in object space
     *  Trace using matrix
     * */

     double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
     int stop = opt_model_->seq_model()->stop_surface();


     // Get entrance pupil diameter and distance

     Path path_obj_to_stop = opt_model_->seq_model()->path(ref_wvl, 0, stop);

     Eigen::Matrix2d system_mat = Eigen::Matrix2d::Identity(2,2);
     Eigen::Matrix2d refraction_mat;
     Eigen::Matrix2d transfer_mat;

     double n_before = path_obj_to_stop[0].rndx;
     double n_after;
     for(int i = 1; i < (int)path_obj_to_stop.size(); i++) {
         double c = path_obj_to_stop[i].srf->profile()->cv();
         n_after = path_obj_to_stop[i].rndx;

         double phi = c*(n_after - n_before);

         refraction_mat = Eigen::Matrix2d::Identity(2,2);
         refraction_mat(1,0) = -phi;

         double d = path_obj_to_stop[i].gap->thi();
         transfer_mat = Eigen::Matrix2d::Identity(2,2);
         transfer_mat(0, 1) = d;

         system_mat = transfer_mat* (refraction_mat * system_mat);

         n_after = n_before;
     }

     Eigen::Vector2d yu_stop({0.0, 1.0}); // u is arbitrary
     Eigen::Vector2d yu_s1; // at surface 1

     // yu_stop = system_mat * yu_s1
     yu_s1 = system_mat.inverse() * yu_stop;

}


void ParaxialModel::paraxial_trace(ParaxialRay& p_ray, ParaxialRay& p_ray_bar, int start, ParaxialRayHtAndSlp start_yu, ParaxialRayHtAndSlp start_yu_bar, double wvl)
{
    p_ray.clear();
    p_ray_bar.clear();

    //double wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
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


    ParaxialRayAtSurface b4_yui;
    b4_yui.ht  = start_yu.ht;
    b4_yui.slp = start_yu.slp;


    ParaxialRayAtSurface b4_yui_bar;
    b4_yui_bar.ht = start_yu_bar.ht;
    b4_yui_bar.slp = start_yu_bar.slp;

    if(start == 1)
    {
        // compute object coords from 1st surface data
        double t0 = before.gap->thi();
        double obj_ht;
        double obj_htb;
        if(std::isinf(t0)){
            obj_ht = start_yu.ht;
            obj_htb = start_yu_bar.ht;
        }else{
            obj_ht = start_yu.ht - t0*start_yu.slp;
            obj_htb = start_yu_bar.ht - t0*start_yu_bar.slp;
        }

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

    b4_yui.n = n_before;
    b4_yui_bar.n = n_before;

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
            double cur_ht;
            double cur_htb;
            if(std::isinf(t)){
                cur_ht = b4_yui.ht;
                cur_htb = b4_yui_bar.ht;
            }else{
                cur_ht = b4_yui.ht + t*b4_yui.slp;
                cur_htb = b4_yui_bar.ht + t*b4_yui_bar.slp;
            }

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
            yu.n = n_before;

            //ParaxialRayComponent yu_bar;
            ParaxialRayAtSurface yu_bar;
            yu_bar.ht = cur_htb;
            yu_bar.slp = cur_slpb;
            yu_bar.aoi = aoi_bar;
            yu_bar.n = n_before;


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


}


