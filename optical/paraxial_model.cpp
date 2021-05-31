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
    fod_ = compute_first_order();
}

FirstOrderData ParaxialModel::first_order_data() const
{
    return fod_;
}

FirstOrderData ParaxialModel::compute_first_order()
{
    FirstOrderData fod;
    auto seq_model = opt_model_->seq_model();
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();

    int stop = seq_model->stop_surface();
    int last_surf_idx = seq_model->surface_count() -1 -1;

    double n0 = seq_model->gap(0)->medium()->rindex(ref_wvl);
    double nk = seq_model->gap(-1)->medium()->rindex(ref_wvl);

    fod.obj_dist = seq_model->gap(0)->thi();
    fod.img_dist = seq_model->gap(-1)->thi();

    Eigen::Matrix2d sys_mat = compute_system_matrix(1, last_surf_idx, ref_wvl);
    double Ak = -sys_mat(0,1);
    double Bk = sys_mat(0,0);
    double Ck = sys_mat(1,1);
    double Dk = -sys_mat(1,0);

    fod.efl = 1.0/Ak;
    fod.ffl = - n0*Bk/Ak;
    fod.bfl = nk*Ck/Ak;

    fod.enp_dist = compute_entrance_pupil_distance();
    fod.exp_dist = compute_exit_pupil_distance();

    compute_paraxial_ray(ax_, pr_, ref_wvl);

    double y1 = ax_.at(1).ht;
    double u1 = ax_.at(0).slp;
    double ubar1 = pr_.at(0).slp;
    double ybar1 = pr_.at(1).ht;

    fod_.opt_inv = n0*(y1*ubar1 - ybar1*u1);

    double ubar0 = pr_.at(0).slp;
    fod_.enp_radius = abs(fod_.opt_inv/(n0*ubar0));

    double uk_prime = ax_.at_last().slp;
    fod_.fno = -1.0/(2.0*nk*uk_prime);
    fod_.m = (n0*u1)/(nk*uk_prime);
    fod_.red = 1.0/fod_.m;

    fod_.img_ht = pr_.at_image().ht;
    fod_.obj_ang = pr_.at(0).slp * 180/M_PI;

    double ubark_prime = pr_.at_last().slp;
    fod_.exp_radius = abs( fod_.opt_inv/(nk*ubark_prime) );

    return fod;
}



void ParaxialModel::compute_paraxial_ray(ParaxialRay& ax_ray, ParaxialRay& pr_ray, double wvl)
{
    /*
     * Entrance pupil location and magnification are calculated using reference wavelength.
     */

    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    auto seq_model = opt_model_->seq_model();
    int stop = seq_model->stop_surface();

    double n0 = seq_model->gap(0)->medium()->rindex(wvl);
    double nk = seq_model->gap(-1)->medium()->rindex(wvl);

    ParaxialPath path = forward_paraxial_path(0, seq_model->image_surface(), wvl);

    double enp_dist = compute_entrance_pupil_distance();
    double obj_dist = seq_model->gap(0)->thi();
    double obj2enp_dist = obj_dist + enp_dist;
    double img_dist = seq_model->gap(-1)->thi();
    double exp_dist = compute_exit_pupil_distance();

    ax_ray.clear();
    pr_ray.clear();

    // axial ray at object and 1st surface
    int pupil_type = opt_model_->optical_spec()->pupil_spec()->pupil_type();

    if (pupil_type == PupilType::EPD) {

        double epd = opt_model_->optical_spec()->pupil_spec()->value();
        double enp_radius = epd/2;
        double c1 = path[1].c;
        double n1 = path[0].n;
        double n1_prime = path[1].n;
        double y0 = 0.0;
        double u0_prime = enp_radius / obj2enp_dist;
        double u1 = u0_prime;
        double y1;
        if(std::isinf(obj_dist)){
            y1 = enp_radius;
        }else{
            y1 = y0 + u0_prime*obj_dist;
        }
        double i1 = u1 + y1*c1;
        double i1_prime = i1*(n1/n1_prime);
        double u1_prime = i1_prime - y1*c1;
        double i0 = u1;

        // object
        ax_ray.append(y0, u0_prime, i0, n0);

        // 1st surface
        ax_ray.append(y1, u1_prime, i1, n1);

    }else if (pupil_type == PupilType::FNO) {

        double Fno = opt_model_->optical_spec()->pupil_spec()->value();
        double uk = -1.0/(2.0 * Fno);
        double yk = img_dist*uk;
        Eigen::Vector2d nu_y_k({nk*uk, yk});
        Eigen::Matrix2d sys_mat = compute_system_matrix(1, seq_model->last_surface(), ref_wvl);
        Eigen::Vector2d nu_y_1 = sys_mat.inverse() * nu_y_k;
        double nu1 = nu_y_1(0);
        double u1 = nu1/n0;
        double y1 = nu_y_1(1);
        double nu0 = nu1;
        double c1 = path[1].c;
        double i1 = u1 + y1*c1;
        double u1_prime = i1-y1*c1;
        double n1 = path[0].n;
        double n1_prime = path[1].n;
        double i0 = u1;
        double u0_prime = u1;
        double y0 = 0.0;

        // object
        ax_ray.append(y0, u0_prime, i0, n0);

        // 1st surface
        ax_ray.append(y1, u1_prime, i1, n1);


    }else if (pupil_type == PupilType::NA) {

        double NA = opt_model_->optical_spec()->pupil_spec()->value();
        double nuk = NA;
        double uk = nuk/nk;
        double yk = img_dist*uk;
        Eigen::Vector2d nu_y_k({nk*uk, yk});
        Eigen::Matrix2d sys_mat = compute_system_matrix(1, seq_model->last_surface(), ref_wvl);
        Eigen::Vector2d nu_y_1 = sys_mat.inverse() * nu_y_k;
        double nu1 = nu_y_1(0);
        double u1 = nu1/n0;
        double y1 = nu_y_1(1);
        double nu0 = nu1;
        double c1 = path[1].c;
        double i1 = u1 + y1*c1;
        double u1_prime = i1-y1*c1;
        double n1 = path[0].n;
        double n1_prime = path[1].n;
        double i0 = u1;
        double u0_prime = u1;
        double y0 = 0.0;

        // object
        ax_ray.append(y0, u0_prime, i0, n0);

        // 1st surface
        ax_ray.append(y1, u1_prime, i1, n1);

    }else if (pupil_type == PupilType::NAO) {
        double NAO = opt_model_->optical_spec()->pupil_spec()->value();
        double nu0 = NAO;
        double u0 = nu0/n0;
        double y0 = 0.0;
        double u1 = u0;
        double y1 = obj2enp_dist * u0;
        double c1 = path[1].c;
        double i1 = u1 + y1*c1;
        double u1_prime = i1-y1*c1;
        double n1 = path[0].n;
        double n1_prime = path[1].n;
        double i0 = u1;
        double u0_prime = u1;

        // object
        ax_ray.append(y0, u0_prime, i0, n0);

        // 1st surface
        ax_ray.append(y1, u1_prime, i1, n1);

    }


    // principle ray at object and 1st surface
    int field_type = opt_model_->optical_spec()->field_of_view()->field_type();

    if (field_type == FieldType::OBJ_ANG) {

        double c1 = path[1].c;
        double n1 = path[0].n;
        double n1_prime = path[1].n;
        double ubar0_prime = tan( opt_model_->optical_spec()->field_of_view()->max_field() * M_PI/180 );
        double ubar1 = ubar0_prime;
        double ybar0 = -obj2enp_dist*ubar0_prime;
        double ybar1;
        if(std::isinf(obj_dist)){
            ybar1 = -ubar1*enp_dist;
        }else{
            ybar1 = ybar0 + ubar0_prime*obj_dist;
        }
        double ibar1 = ubar1 + ybar1*c1;
        double ibar1_prime = ibar1*(n1/n1_prime);
        double ibar0 = ubar1;
        double ubar1_prime = ibar1_prime - ybar1*c1;

        // object
        pr_ray.append(ybar0, ubar0_prime, ibar0, n0);

        // 1st surface
        pr_ray.append(ybar1, ubar1_prime, ibar1, n1);

    }else if (field_type == FieldType::IMG_HT) {
        double c1 = path[1].c;
        double obj_dist = seq_model->gap(0)->thi();
        double img_ht = opt_model_->optical_spec()->field_of_view()->max_field();
        double img_dist = fod_.img_dist;
        double exp2img_dist = fod_.img_dist - fod_.exp_dist;
        double ubark_prime = img_ht/exp2img_dist;
        double ybark = img_ht - img_dist*ubark_prime;
        double nk_prime = seq_model->gap(-1)->medium()->rindex(ref_wvl);
        Eigen::Vector2d nubar_ybar_k_prime({nk_prime*ubark_prime, ybark});
        Eigen::Vector2d nubar_ybar_1 = compute_system_matrix(1, seq_model->last_surface(), ref_wvl).inverse() * nubar_ybar_k_prime;
        double nubar1 = nubar_ybar_1(0);
        double ubar1 = nubar1 / n0;
        double ybar1  = nubar_ybar_1(1);
        double ybar0 = ybar1 + obj_dist*ubar1;
        double ubar0_prime = ubar1;
        double ibar1 = ubar1 + ybar1*c1;
        double ibar0 = ubar1;
        double ubar1_prime = ibar1-ybar1*c1;
        double n1 = path[0].n;
        double n1_prime = path[1].n;

        // object
        pr_ray.append(ybar0, ubar0_prime, ibar0, n0);

        // 1st surface
        pr_ray.append(ybar1, ubar1_prime, ibar1, n1);

    }else if(field_type == FieldType::OBJ_HT) {
        double c1 = path[1].c;
        double obj_ht = opt_model_->optical_spec()->field_of_view()->max_field();
        double obj_dist = seq_model->gap(0)->thi();
        double enp_dist = compute_entrance_pupil_distance();
        double obj2enp_dist = obj_dist + enp_dist;
        double ybar0 = obj_ht;
        double ubar0_prime = obj_ht/obj2enp_dist;
        double ubar1 = ubar0_prime;
        double ybar1 = ubar1 * enp_dist;
        double ibar1 = ubar1 + ybar1*c1;
        double ibar0 = ubar1;
        double ubar1_prime = ibar1-ybar1*c1;
        double n1 = path[0].n;
        double n1_prime = path[1].n;

        // object
        pr_ray.append(ybar0, ubar0_prime, ibar0, n0);

        // 1st surface
        pr_ray.append(ybar1, ubar1_prime, ibar1, n1);
    }

    // trace paraxial rays for the resting surfaces
    double u1_prime = ax_ray.at(1).slp; // before
    double y1 = ax_ray.at(1).ht;
    double d1 = path[1].t;
    double y = y1 + d1*u1_prime; //y2
    double u = u1_prime;

    double ybar1 = pr_ray.at(1).ht;
    double ubar1_prime = pr_ray.at(1).slp;
    double ybar = ybar1 + d1*ubar1_prime; // ybar2
    double ubar = ubar1_prime;
    double n = path[1].n;

    for(int k = 2; k < (int)path.size(); k++) {

        double c = path[k].c;
        double n_prime = path[k].n;

        // axial ray
        double i = u + y*c;
        double i_prime = i*(n/n_prime);
        double u_prime = i_prime - y*c;

        ax_ray.append(y, u_prime, i, n);


        // principle ray
        double ibar = ubar + ybar*c;
        double ibar_prime = ibar*(n/n_prime);
        double ubar_prime = ibar_prime - ybar*c;

        pr_ray.append(ybar, ubar_prime, ibar, n);

        double d = path[k].t;
        y = y + u_prime*d;
        ybar = ybar + ubar_prime*d;
        u = u_prime;
        ubar = ubar_prime;
        n = n_prime;
    }

}

/*
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


    double enp_dist = compute_entrance_pupil_distance();

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
        if(abs(start_yu.slp) < __DBL_EPSILON__){
            obj_ht = start_yu.ht;
        }else{
            obj_ht = start_yu.ht - t0*start_yu.slp;
        }

        if(abs(start_yu_bar.slp) < __DBL_EPSILON__){
            obj_htb = start_yu_bar.ht;
        }else{
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
            if(abs(b4_yui.slp) < __DBL_EPSILON__){ // u == 0.0
                cur_ht = b4_yui.ht; 
            }else{
                cur_ht = b4_yui.ht + t*b4_yui.slp;
            }

            if(abs(b4_yui_bar.slp) < __DBL_EPSILON__){ // u_bar == 0.0
                cur_htb = b4_yui_bar.ht;
            }else{
                if(std::isinf(t)){
                    cur_htb = -b4_yui_bar.slp * enp_dist;
                }else{
                    cur_htb = b4_yui_bar.ht + t*b4_yui_bar.slp;
                }
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
*/


Eigen::Matrix2d ParaxialModel::compute_system_matrix(int start, int end, double wvl)
{
    Eigen::Matrix2d system_mat = Eigen::Matrix2d::Identity(2,2);
    Eigen::Matrix2d refraction_mat;
    Eigen::Matrix2d transfer_mat;

    ParaxialPath path;

    if(start <= end){
        path = forward_paraxial_path(start, end, wvl);
    }else{
        path = reverse_paraxial_path(start, end, wvl);
    }

    double n_before;
    if(start > 0){
        n_before = opt_model_->seq_model()->gap(start-1)->medium()->rindex(wvl);
    }else{
        n_before = 1.0;
    }
    double n_after;
    double c;
    double z_dir_after = 1.0;
    double phi;
    int num_srfs = (int)path.size();

    for(int i = 0; i < ((int)path.size()-1); i++) {
        c = path[i].c;
        z_dir_after = path[i].z_dir;

        if(z_dir_after > 0){
            n_after = path[i].n;
        }else{
            n_after = -path[i].n;
        }

        phi = c*(n_after - n_before);

        refraction_mat = Eigen::Matrix2d::Identity(2,2);
        refraction_mat(0,1) = -phi;

        transfer_mat = Eigen::Matrix2d::Identity(2,2);
        transfer_mat(1,0) = path[i].t/n_after;


        system_mat = transfer_mat* (refraction_mat * system_mat);

        n_before = n_after;
    }

    // refraction at the end surface
    c = path.back().c;
    if(z_dir_after > 0){
        n_after = path.back().n;
    }else{
        n_after = -path.back().n;
    }
    phi = c*(n_after - n_before);
    refraction_mat = Eigen::Matrix2d::Identity(2,2);
    refraction_mat(0,1) = -phi;
    system_mat = refraction_mat*system_mat;

    return system_mat;
}




ParaxialPath ParaxialModel::forward_paraxial_path(int start, int end, double wvl)
{
    assert(start <= end);

    SequentialModel* seq_model = opt_model_->seq_model();
    int num_gaps = seq_model->gap_count();

    ParaxialPathComponent pr_path_comp;
    ParaxialPath pr_path;

    for(int i = start; i <= end; i++) {
        pr_path_comp.si = i;
        pr_path_comp.c = seq_model->surface(i)->profile()->cv();

        if( i < num_gaps ){
            pr_path_comp.t = seq_model->gap(i)->thi();
            pr_path_comp.n = seq_model->gap(i)->medium()->rindex(wvl);
        }else{
            pr_path_comp.t = 0.0;
            pr_path_comp.n = 1.0;
        }

        pr_path.push_back(pr_path_comp);
    }

    return pr_path;
}

ParaxialPath ParaxialModel::reverse_paraxial_path(int start, int end, double wvl)
{
    assert(start >= end);

    SequentialModel* seq_model = opt_model_->seq_model();

    ParaxialPathComponent pr_path_comp;
    ParaxialPath pr_path;

    for(int i = start; i >= end; i--) {
        pr_path_comp.si = i;
        pr_path_comp.c  = - seq_model->surface(i)->profile()->cv();

        if( i > 0 ){
            pr_path_comp.t = seq_model->gap(i-1)->thi();
            pr_path_comp.n = seq_model->gap(i-1)->medium()->rindex(wvl);
        }else{
            pr_path_comp.t = 0.0;
            pr_path_comp.n = 1.0;
        }

        pr_path.push_back(pr_path_comp);
    }

    return pr_path;
}

double ParaxialModel::compute_entrance_pupil_distance()
{
    auto seq_model = opt_model_->seq_model();
    int stop = seq_model->stop_surface();

    assert(stop > 0);

    if(stop == 1) {
        return 0.0;
    }

    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    double n0 = seq_model->gap(0)->medium()->rindex(ref_wvl);

    // entrance pupil distance
    Eigen::Matrix2d sys_mat_stop_s1 = compute_system_matrix(stop, 1, ref_wvl);
    Eigen::Vector2d nu_y_stop({1.0, 0.0});
    Eigen::Vector2d nu_y_s1 = sys_mat_stop_s1 * nu_y_stop;
    double enp_dist = nu_y_s1(1)/(n0*nu_y_s1(0));

    return enp_dist;
}

double ParaxialModel::compute_exit_pupil_distance()
{
    auto seq_model = opt_model_->seq_model();
    int last_surf_idx = seq_model->surface_count() -1-1;
    int stop = seq_model->stop_surface();

    assert(0 < stop && stop <= last_surf_idx );

    if(stop == last_surf_idx) {
        return 0.0;
    }

    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    double nk = seq_model->gap(-1)->medium()->rindex(ref_wvl);

    Eigen::Matrix2d sys_mat_stop_last = compute_system_matrix(stop, last_surf_idx, ref_wvl);
    Eigen::Vector2d nu_y_stop({1.0, 0.0});
    Eigen::Vector2d nu_y_last_prime = sys_mat_stop_last * nu_y_stop;
    double exp_dist = - nu_y_last_prime(1)/(nk*nu_y_last_prime(0));

    return exp_dist;
}

void ParaxialModel::nodal_points(int start, int end, double *pp1, double *pp2)
{
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    Eigen::Matrix2d sys_mat = compute_system_matrix(start, end, ref_wvl);

    // Kingslake system matrix
    double Ak = -sys_mat(0,1);
    double Bk = sys_mat(0,0);
    double Ck = sys_mat(1,1);

    if(abs(Ak) < __DBL_EPSILON__) {
        *pp1 = 1000; // large value
        *pp2 = 1000;
    }else {
        *pp1 = (Bk - 1)/Ak;
        *pp2 = (Ck - 1)/Ak;
    }
}
