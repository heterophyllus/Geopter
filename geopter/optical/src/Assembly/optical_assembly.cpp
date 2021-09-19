#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <algorithm>
#include <iomanip>

#include "Assembly/optical_assembly.h"
#include "Assembly/spherical.h"

#include "Material/air.h"

#include "Spec/spectral_line.h"
#include "Spec/optical_spec.h"

using namespace geopter;

OpticalAssembly::OpticalAssembly()
{

}

OpticalAssembly::~OpticalAssembly()
{
    clear();
}


void OpticalAssembly::update_model()
{
    set_local_transforms();

    constexpr int ref_srf = 1;
    set_global_transforms(ref_srf);
}

void OpticalAssembly::clear()
{
    if(!interfaces_.empty())
    {
        for(auto &s : interfaces_){
            s.reset();
        }
        interfaces_.clear();
    }

    if(!gaps_.empty())
    {
        for(auto &g : gaps_){
            g.reset();
        }
        gaps_.clear();
    }
}

void OpticalAssembly::create_minimun_assembly()
{
    clear();

    // add object interface and gap
    auto s_obj = std::make_shared<Surface>("Obj");
    interfaces_.push_back(s_obj);

    auto air = std::make_shared<Air>();
    auto g = std::make_shared<Gap>(0.0, air);
    gaps_.push_back(g);

    // add stop interface and gap
    auto s_stop = std::make_shared<Surface>("Stop");
    interfaces_.push_back(s_stop);

    auto g_stop = std::make_shared<Gap>(0.0, air);
    gaps_.push_back(g_stop);

    stop_surface_ = 1;


    // add image interface and dummy gap
    auto s_img = std::make_shared<Surface>("Img");
    interfaces_.push_back(s_img);

    auto g_img = std::make_shared<Gap>(0.0, air);
    gaps_.push_back(g_img);

}

Surface* OpticalAssembly::surface(int i) const
{
    if( i < (int)interfaces_.size() ) {
        return interfaces_[i].get();
    }else{
        // throw error
        return nullptr;
    }
}

Gap* OpticalAssembly::gap(int i) const
{
    if( i < (int)gaps_.size() ) {
        return gaps_[i].get();
    }else{
        // throw error
        return nullptr;
    }
}

int OpticalAssembly::surface_count() const
{
    return interfaces_.size();
}

int OpticalAssembly::gap_count() const
{
    return gaps_.size() - 1;
}

int OpticalAssembly::stop_index() const
{
    return stop_surface_;
}

int OpticalAssembly::image_index() const
{
    return interfaces_.size() - 1;
}

Gap* OpticalAssembly::image_space_gap() const
{
    int num_gaps = gaps_.size();
    if(num_gaps == 0){
        return nullptr;
    }
    return gaps_[num_gaps-1-1].get();
}

void OpticalAssembly::set_stop(int i)
{
    stop_surface_ = i;
}

void OpticalAssembly::set_object_distance(double t0)
{
    if(!gaps_.empty()){
        gaps_[0]->set_thi(t0);
    }
}

void OpticalAssembly::insert_dummy(int i)
{
    // update stop index
    if( i <= stop_surface_){
        stop_surface_ += 1;
    }

    // create a new standard surface
    auto s = std::make_shared<Surface>();

    // insert the surface
    auto ifcs_itr = interfaces_.begin();
    std::advance(ifcs_itr, i);
    interfaces_.insert(ifcs_itr, s);

    // create a new 0 air gap
    auto g = std::make_shared<Gap>(0.0, std::make_shared<Air>());

    // insert the gap
    auto gap_itr = gaps_.begin();
    std::advance(gap_itr, i);
    gaps_.insert(gap_itr, g);
}

void OpticalAssembly::remove(int i)
{
    if ( i < (int)gaps_.size() ) {
        auto ifcs_itr = interfaces_.begin();
        interfaces_[i].reset();
        interfaces_.erase(ifcs_itr + i);

        auto gap_itr = gaps_.begin();
        gaps_[i].reset();
        gaps_.erase(gap_itr + i);

        // update stop surface
        if ( i < stop_surface_ ) {
            stop_surface_ -= 1;
        }
    }
}




void OpticalAssembly::add_surface_and_gap(std::shared_ptr<Surface> s, std::shared_ptr<Gap> g)
{
    interfaces_.push_back(s);
    gaps_.push_back(g);
}

/*
void OpticalAssembly::add_surface_and_gap(double r, double t, std::string mat_name)
{
    auto s = std::make_shared<Surface>();
    s->profile()->set_cv(1.0/r);

    auto g = std::make_shared<Gap>();
    g->set_thi(t);

    auto m = parent_opt_sys_->material_lib()->find(mat_name);
    if(m) {
        g->set_material(m);
    }

    // insert surface and gap to just before the image
    //auto s_itr = interfaces_.end() - 1;
    //interfaces_.insert(s_itr, s);
    interfaces_.push_back(s);
    gaps_.push_back(g);

}
*/

void OpticalAssembly::set_local_transforms()
{
    // For the moment, deceneter is not supported

    Eigen::Matrix3d r = Eigen::Matrix3d::Identity(3,3);
    Eigen::Vector3d t = Eigen::Vector3d::Zero(3);
    Transformation tfrm;

    int num_gaps = gaps_.size();
    for (int i = 0; i < num_gaps; i++) {
        //r = rotation to next
        //t(0) = x to next
        //t(1) = y to next
        t(2) = gaps_[i]->thi();
        tfrm.rotation = r;
        tfrm.transfer = t;
        interfaces_[i]->set_local_transform(tfrm);
    }
}

void OpticalAssembly::set_global_transforms(int ref_srf)
{
    assert(ref_srf > 0);

    // For the moment, deceneter is not supported
    Eigen::Matrix3d global_rot = Eigen::Matrix3d::Identity(3,3);

    Transformation tfrm;

    // ref
    tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
    tfrm.transfer = Eigen::Vector3d::Zero(3);
    interfaces_[ref_srf]->set_global_transform(tfrm);

    // s0..ref-1
    for (int i = 0; i < ref_srf; i++) {
        Eigen::Vector3d transfer_cur_to_ref = Eigen::Vector3d::Zero(3);

        for(int j = i; j < ref_srf; j++) {
            Eigen::Vector3d transfer_sj = interfaces_[j]->local_transform().transfer;
            transfer_cur_to_ref += transfer_sj;
        }
        Eigen::Vector3d transfer_ref_to_cur = - transfer_cur_to_ref;

        tfrm.rotation = global_rot;
        tfrm.transfer = transfer_ref_to_cur;
        interfaces_[i]->set_global_transform(tfrm);
    }

    // ref+1..last
    int num_srfs = interfaces_.size();
    for (int i = ref_srf+1; i < num_srfs; i++) {
        Eigen::Vector3d transfer_ref_to_cur = Eigen::Vector3d::Zero(3);

        for(int j = ref_srf; j < i; j++) {
            Eigen::Vector3d transfer_sj = interfaces_[j]->local_transform().transfer;
            transfer_ref_to_cur += transfer_sj;
        }
        tfrm.rotation = global_rot;
        tfrm.transfer = transfer_ref_to_cur;
        interfaces_[i]->set_global_transform(tfrm);
    }
}

/*
void OpticalAssembly::set_semi_diameters()
{
    const int num_srf = interfaces_.size();
    const int num_fld = parent_opt_sys_->optical_spec()->field_of_view()->field_count();
    const int ref_wvl_idx = parent_opt_sys_->optical_spec()->spectral_region()->reference_index();

    // initialize all surface
    for(int si = 0; si < num_srf; si++) {
        interfaces_[si]->set_semi_diameter(0.0);
    }

    // update semi diameter
    Ray chief_ray, mer_upper_ray, mer_lower_ray, sag_upper_ray, sag_lower_ray;

    for(int fi = 0; fi < num_fld; fi++) {

        chief_ray     = parent_opt_sys_->sequential_data()->reference_ray(1,fi,ref_wvl_idx);
        mer_upper_ray = parent_opt_sys_->sequential_data()->reference_ray(2,fi,ref_wvl_idx);
        mer_lower_ray = parent_opt_sys_->sequential_data()->reference_ray(3,fi,ref_wvl_idx);
        sag_upper_ray = parent_opt_sys_->sequential_data()->reference_ray(4,fi,ref_wvl_idx);
        sag_lower_ray = parent_opt_sys_->sequential_data()->reference_ray(5,fi,ref_wvl_idx);

        for(int si = 0; si < num_srf; si++) {

            double chief_ray_ht     = chief_ray.at(si).height();
            double mer_upper_ray_ht = mer_upper_ray.at(si).height();
            double mer_lower_ray_ht = mer_lower_ray.at(si).height();
            double sag_upper_ray_ht = sag_upper_ray.at(si).height();
            double sag_lower_ray_ht = sag_lower_ray.at(si).height();

            double ray_ht_for_cur_fld = std::max({chief_ray_ht, mer_upper_ray_ht, mer_lower_ray_ht, sag_upper_ray_ht, sag_lower_ray_ht});

            double current_sd = interfaces_[si]->semi_diameter();

            if(current_sd < ray_ht_for_cur_fld) {
                interfaces_[si]->set_semi_diameter(ray_ht_for_cur_fld);
            }
        }

    }
    
}
*/


double OpticalAssembly::overall_length(int start, int end)
{
    assert(start <= end);

    double oal = 0.0;
    for(int si = start; si < end; si++){
        oal += gaps_[si]->thi();
    }

    return oal;;
}

void OpticalAssembly::print(std::ostringstream& oss) const
{
    constexpr int idx_w = 4;
    constexpr int val_w = 16;
    constexpr int prec  = 6;

    // header labels
    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "Radius";
    oss << std::setw(val_w) << std::right << "Thickness";
    oss << std::setw(val_w) << std::right << "Medium";
    oss << std::setw(val_w) << std::right << "Index";
    oss << std::setw(val_w) << std::right << "SemiDiam";
    oss << std::setw(val_w) << std::right << "LclTfrm(X)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Y)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Z)";
    oss << std::setw(val_w) << std::right << "GblTfrm(X)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Y)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Z)";
    oss << std::endl;

    int num_srf = interfaces_.size();

    double r, thi, nd, sd;
    std::string mat_name;

    for(int i = 0; i < num_srf; i++) {

        r = interfaces_[i]->profile()->radius();

        if(i < gap_count()){
            thi = gaps_[i]->thi();
            mat_name = gaps_[i]->material()->name();
            nd = gaps_[i]->material()->rindex(SpectralLine::d);
        }else{
            thi = 0.0;
            mat_name = "";
            nd = 1.0;
        }
        
        sd = interfaces_[i]->semi_diameter();

        oss << std::setw(idx_w) << std::right << std::fixed << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << r;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << thi;
        oss << std::setw(val_w) << std::right << std::fixed << mat_name;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << nd;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << sd;

        // local transforms
        Eigen::Vector3d lcl_t = interfaces_[i]->local_transform().transfer;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(2);

        // global transform
        Eigen::Vector3d gbl_t = interfaces_[i]->global_transform().transfer;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(2);

        oss << std::endl;
    }

    oss << std::endl;
}
