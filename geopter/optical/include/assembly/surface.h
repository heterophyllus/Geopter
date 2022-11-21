/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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

#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include <memory>
#include <variant>

#include "assembly/transformation.h"
#include "assembly/aperture.h"
#include "assembly/circular.h"
#include "assembly/decenter_data.h"
#include "solve/solve.h"

#include "profile/surface_profile.h"
#include "profile/spherical.h"
#include "profile/even_polynomial.h"
#include "profile/odd_polynomial.h"

namespace geopter {

/** Optical refractive surface */
class Surface
{
public:
    Surface(std::string lbl="");
    Surface(double r);
    ~Surface();


    std::string interact_mode() const { return interact_mode_;}
    std::string label() const { return label_;}
    inline DecenterData* decenter() const;

    inline Solve* solve() const;

    std::string profile_name() const{
        return std::visit([&](auto p){ return p.name();}, profile_);
    }

    double cv() const{ return std::visit([](auto &p){ return p.cv();}, profile_);}

    double radius() const {
        return std::visit([](auto &p){ return p.radius();}, profile_);
    }

    void set_radius(double r){
        std::visit([&](auto &p){ p.set_radius(r);}, profile_);
    }

    bool intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir){
        return std::visit([&](auto p){ return p.intersect(pt, distance, p0, dir);}, profile_);
    }

    Eigen::Vector3d normal(const Eigen::Vector3d& pt) const{
        return std::visit([&](auto p){ return p.normal(pt);}, profile_);
    }


    double sag(double x, double y){
        return std::visit([&](auto p){ return p.sag(x,y);}, profile_);
    }

    template<class P>
    auto profile(){
        return std::get_if< SurfaceProfile<P> >(&profile_);
    }

    template<class P>
    bool is_profile(){
        if(std::get_if< SurfaceProfile<P> >(&profile_)){
            return true;
        }else{
            return false;
        }
    }

    template<class Shape>
    bool is_aperture(){
        if(std::get_if< Aperture<Shape> >(&clear_aperture_)){
            return true;
        }else{
            return false;
        }
    }

    template<class Shape>
    auto clear_aperture(){
        return std::get_if< Aperture<Shape> >(&clear_aperture_);
    }

    /** Return aperture shape name. If no aperture is set, returns "None" */
    std::string aperture_shape() const;

    /** Maximum size of the interface.
      * If an aperture is set, this function returns the aperture max dimension.
      * Otherwise, it returns valid semi-diameter.
      */
    double max_aperture() const;

    /** Maximum valid semi-diameter where the reference rays pass */
    double semi_diameter() const { return semi_diameter_;}

    /** Returns true if the given point(x,y) is inside of aperture */
    bool point_inside(double x, double y) const;
    bool point_inside(const Eigen::Vector2d& pt) const;

    const Transformation& local_transform() const { return lcl_tfrm_;}
    const Transformation& global_transform() const { return gbl_tfrm_;}

    template<class Profile>
    void set_profile(double cv, double k=0.0, const std::vector<double>& coefs=std::vector<double>(10, 0.0)){
        if constexpr (std::is_same_v<Profile, Spherical>){
            profile_ = SurfaceProfile<Spherical>(cv);
        }else {
            profile_ = SurfaceProfile<Profile>(cv, k, coefs);
        }
    }


    void set_label(std::string lbl) { label_ = lbl;}

    template<class Shape>
    void set_clear_aperture(double x_dimension, double y_dimension){
        if constexpr (std::is_same_v<Shape, NoneAperture>){
            clear_aperture_ = Aperture<NoneAperture>();
        }else if(std::is_same_v<Shape, Circular>){
            clear_aperture_ = Aperture<Circular>(x_dimension, y_dimension);
        }
    }

    template<class Shape>
    void set_edge_aperture(double x_dimension, double y_dimension){
        if constexpr (std::is_same_v<Shape, NoneAperture>){
            edge_aperture_ = Aperture<NoneAperture>();
        }else if(std::is_same_v<Shape, Circular>){
            edge_aperture_ = Aperture<Circular>(x_dimension, y_dimension);
        }
    }

    /** Remove all clear apertures from the surface */
    void remove_clear_aperture();

    void set_semi_diameter(double sd){ semi_diameter_ = sd;}

    void set_local_transform(const Transformation& tfrm){ lcl_tfrm_ = tfrm; }
    void set_global_transform(const Transformation& tfrm) { gbl_tfrm_ = tfrm;}

    inline void set_solve(std::unique_ptr<Solve> solve);
    inline void remove_solve();

    inline bool has_solve() const;

    void update();

    void print();
    void print(std::ostringstream& oss);

protected:
    std::string label_;
    std::string interact_mode_;

    double semi_diameter_;

    std::variant<SurfaceProfile<Spherical>, SurfaceProfile<EvenPolynomial>, SurfaceProfile<OddPolynomial> > profile_;

    std::variant<Aperture<NoneAperture>, Aperture<Circular>> edge_aperture_;
    std::variant<Aperture<NoneAperture>, Aperture<Circular>> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    std::unique_ptr<Solve> solve_;

    /** transform to next interface */
    Transformation lcl_tfrm_;

    /** global transform against the reference interface */
    Transformation gbl_tfrm_;
};



DecenterData* Surface::decenter() const
{
    return decenter_.get();
}


Solve* Surface::solve() const
{
    return solve_.get();
}



void Surface::set_solve(std::unique_ptr<Solve> solve)
{
    solve_ = std::move(solve);
}

void Surface::remove_solve()
{
    solve_.reset();
    solve_ = nullptr;
}

bool Surface::has_solve() const
{
    if(solve_) return true;
    return false;
}

} //namespace geopter

#endif // SURFACE_H
