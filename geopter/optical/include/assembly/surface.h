/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
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
    Surface();
    ~Surface();

    std::string InteractMode() const { return interact_mode_;}
    std::string Label() const { return label_;}
    DecenterData* Decenter() const { return decenter_.get(); }

    Solve* GetSolve() const { return solve_.get(); }

    std::string ProfileName() const{
        return std::visit([&](auto p){ return p.Name();}, profile_);
    }

    double Curvature() const{ return std::visit([](auto &p){ return p.Curvature();}, profile_);}

    double Radius() const {
        return std::visit([](auto &p){ return p.Radius();}, profile_);
    }

    void SetRadius(double r){
        std::visit([&](auto &p){ p.SetRadius(r);}, profile_);
    }

    bool Intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir){
        return std::visit([&](auto p){ return p.Intersect(pt, distance, p0, dir);}, profile_);
    }

    Eigen::Vector3d Normal(const Eigen::Vector3d& pt) const{
        return std::visit([&](auto p){ return p.Normal(pt);}, profile_);
    }

    double Sag(double x, double y){
        return std::visit([&](auto p){ return p.Sag(x,y);}, profile_);
    }

    template<class P>
    auto Profile(){
        return std::get_if< SurfaceProfile<P> >(&profile_);
    }

    template<class P>
    bool IsProfile(){
        if(std::get_if< SurfaceProfile<P> >(&profile_)){
            return true;
        }else{
            return false;
        }
    }

    template<class Shape>
    bool IsAperture(){
        if(std::get_if< Aperture<Shape> >(&clear_aperture_)){
            return true;
        }else{
            return false;
        }
    }

    template<class Shape>
    auto GetClearAperture(){
        return std::get_if< Aperture<Shape> >(&clear_aperture_);
    }

    /** Return aperture shape name. If no aperture is set, returns "None" */
    std::string ApertureShape() const;

    /** Maximum size of the interface.
      * If an aperture is set, this function returns the aperture max dimension.
      * Otherwise, it returns valid semi-diameter.
      */
    double MaxAperture() const;

    /** Maximum valid semi-diameter where the reference rays pass */
    double SemiDiameter() const { return semi_diameter_;}

    /** Returns true if the given point(x,y) is inside of aperture */
    bool PointInside(double x, double y) const {
        return std::visit([&](auto ap){ return ap.PointInside(x,y); }, clear_aperture_);
    }
    bool PointInside(const Eigen::Vector2d& pt) const{
        return std::visit([&](auto ap){ return ap.PointInside(pt(0), pt(1)); }, clear_aperture_);
    }

    const Transformation& LocalTransform() const { return lcl_tfrm_;}
    const Transformation& GlobalTransform() const { return gbl_tfrm_;}

    template<class Profile>
    void SetProfile(double cv, double k=0.0, const std::vector<double>& coefs=std::vector<double>(10, 0.0)){
        if constexpr (std::is_same_v<Profile, Spherical>){
            profile_ = SurfaceProfile<Spherical>(cv);
        }else {
            profile_ = SurfaceProfile<Profile>(cv, k, coefs);
        }
    }


    void SetLabel(std::string lbl) { label_ = lbl;}

    template<class Shape>
    void SetClearAperture(double x_dimension, double y_dimension){
        if constexpr (std::is_same_v<Shape, NoneAperture>){
            clear_aperture_ = Aperture<NoneAperture>();
        }else if(std::is_same_v<Shape, Circular>){
            clear_aperture_ = Aperture<Circular>(x_dimension, y_dimension);
        }
    }


    /** Remove all clear apertures from the surface */
    void RemoveClearAperture();

    void SetSemiDiameter(double sd){ semi_diameter_ = sd;}

    void SetLocalTransform(const Transformation& tfrm){ lcl_tfrm_ = tfrm; }
    void SetGlobalTransform(const Transformation& tfrm) { gbl_tfrm_ = tfrm;}

    void SetSolve(std::unique_ptr<Solve> solve) { solve_ = std::move(solve); }
    void RemoveSolve() { solve_.reset();
                         solve_ = nullptr;}

    bool HasSolve() const { if(solve_) return true; return false; }

    void Update();

    void Print();
    void Print(std::ostringstream& oss);

protected:
    std::string label_;
    std::string interact_mode_;

    double semi_diameter_;

    std::variant<SurfaceProfile<Spherical>, SurfaceProfile<EvenPolynomial>, SurfaceProfile<OddPolynomial> > profile_;

    std::variant<Aperture<NoneAperture>, Aperture<Circular>> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    std::unique_ptr<Solve> solve_;

    /** transform to next interface */
    Transformation lcl_tfrm_;

    /** global transform against the reference interface */
    Transformation gbl_tfrm_;
};



} //namespace geopter

#endif // SURFACE_H
