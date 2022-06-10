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
#include <math.h>

#include <iostream>
#include <algorithm>
#include <iomanip>

#include "assembly/optical_assembly.h"
#include "material/material_library.h"
#include "spec/spectral_line.h"
#include "spec/optical_spec.h"

using namespace geopter;

int OpticalAssembly::num_surfs_ = 0;

OpticalAssembly::OpticalAssembly()
{

}

OpticalAssembly::~OpticalAssembly()
{
    clear();
}


void OpticalAssembly::update_model()
{
    // update transforms
    set_local_transforms();
    set_global_transforms(1);

    num_surfs_ = interfaces_.size();
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

    num_surfs_ = 0;
}

void OpticalAssembly::create_minimun_assembly()
{
    clear();

    // add object interface and gap
    auto s_obj = std::make_unique<Surface>("Obj");
    interfaces_.push_back(std::move(s_obj));

    //auto air = std::make_shared<Air>();
    auto air = MaterialLibrary::air();
    auto g = std::make_unique<Gap>(0.0, air);
    gaps_.push_back(std::move(g));

    // add stop interface and gap
    auto s_stop = std::make_unique<Surface>("Stop");
    interfaces_.push_back(std::move(s_stop));

    auto g_stop = std::make_unique<Gap>(0.0, air);
    gaps_.push_back(std::move(g_stop));

    stop_index_ = 1;


    // add image interface and dummy gap
    auto s_img = std::make_unique<Surface>("Img");
    interfaces_.push_back(std::move(s_img));

    auto g_img = std::make_unique<Gap>(0.0, air);
    gaps_.push_back(std::move(g_img));

    current_surface_index_ = 2;

    num_surfs_ = interfaces_.size();
}


Gap* OpticalAssembly::image_space_gap() const
{
    if(gaps_.empty()){
        return nullptr;
    }else{
        int num_gaps = gaps_.size();
        return gaps_[num_gaps-1-1].get();
    }

}

void OpticalAssembly::insert(int i)
{
    this->insert(i, std::numeric_limits<double>::infinity(), 0.0, "AIR");
}

void OpticalAssembly::insert(int i, double r, double t, const std::string &mat_name)
{
    bool is_appending = false; // append after image

    if(i < 0){
        //std::cout << "insert before object" << std::endl;
        i = 0;
    }else if( i >= (int)interfaces_.size()){
        //std::cout << "insert after image" << std::endl;
        is_appending = true;
    }

    if(interfaces_.empty()){
        is_appending = true;
    }

    // update stop index
    if( i <= stop_index_){
        stop_index_ += 1;
    }

    // insert the surface
    auto s = std::make_unique<Surface>(r);

    if(is_appending){
        interfaces_.push_back(std::move(s));
    }
    else{
        auto ifcs_itr = interfaces_.begin();
        std::advance(ifcs_itr, i);
        interfaces_.insert(ifcs_itr, std::move(s));
    }

    // search material
    auto m = MaterialLibrary::find(mat_name);

    // insert the gap
    auto g = std::make_unique<Gap>(t,m);

    if(is_appending){
        gaps_.push_back(std::move(g));
    }
    else{
        auto gap_itr = gaps_.begin();
        std::advance(gap_itr, i);
        gaps_.insert(gap_itr, std::move(g));
    }

    current_surface_index_ = i;

    num_surfs_ = interfaces_.size();
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
        if ( i < stop_index_ ) {
            stop_index_ -= 1;
        }

        //update current
        if (i < current_surface_index_){
            current_surface_index_ -= 1;
        }
    }
    num_surfs_ = interfaces_.size();
}


void OpticalAssembly::add_surface_and_gap()
{
    auto s = std::make_unique<Surface>();
    interfaces_.push_back(std::move(s));

    auto g = std::make_unique<Gap>();
    gaps_.push_back(std::move(g));

    current_surface_index_ = interfaces_.size() -1;

    num_surfs_ = interfaces_.size();
}



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


double OpticalAssembly::overall_length(int start, int end)
{
    assert(start <= end);

    double oal = 0.0;
    for(int si = start; si < end; si++){
        oal += gaps_[si]->thi();
    }

    return oal;
}

void OpticalAssembly::print(std::ostringstream& oss) const
{
    constexpr int idx_w = 4;
    constexpr int val_w = 16;
    constexpr int prec  = 6;

    // header labels
    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "Label";
    oss << std::setw(val_w) << std::right << "Radius";
    oss << std::setw(val_w) << std::right << "Thickness";
    oss << std::setw(val_w) << std::right << "Material";
    oss << std::setw(val_w) << std::right << "Index";
    oss << std::setw(val_w) << std::right << "Abbe-d";
    oss << std::setw(val_w) << std::right << "Aperture";
    oss << std::setw(val_w) << std::right << "SemiDiam";
    oss << std::setw(val_w) << std::right << "LclTfrm(X)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Y)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Z)";
    oss << std::setw(val_w) << std::right << "GblTfrm(X)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Y)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Z)";
    oss << std::endl;

    int num_srf = interfaces_.size();

    double r, thi, nd, vd, sd;
    vd = 0.0;
    std::string mat_name, label, aperture_type;

    for(int i = 0; i < num_srf; i++) {

        r             = interfaces_[i]->profile()->radius();
        label         = interfaces_[i]->label();
        sd            = interfaces_[i]->semi_diameter();
        aperture_type = interfaces_[i]->aperture_shape();

        if(aperture_type == "None"){
            aperture_type = "-";
        }

        if(i < gap_count()){
            thi      = gaps_[i]->thi();
            mat_name = gaps_[i]->material()->name();
            nd       = gaps_[i]->material()->rindex(SpectralLine::d);
            vd       = gaps_[i]->material()->abbe_d();
        }else{
            thi = 0.0;
            mat_name = "";
            nd = 1.0;
        }
        
        oss << std::setw(idx_w) << std::right << std::fixed << i;
        oss << std::setw(val_w) << std::right << std::fixed << label;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << r;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << thi;
        oss << std::setw(val_w) << std::right << std::fixed << mat_name;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << nd;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << vd;
        oss << std::setw(val_w) << std::right << std::fixed << aperture_type;
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
