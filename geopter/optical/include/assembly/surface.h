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

#include "assembly/transformation.h"
#include "assembly/aperture.h"
#include "assembly/circular.h"
#include "assembly/decenter_data.h"
#include "assembly/solve.h"

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

    inline std::string interact_mode() const;
    inline std::string label() const;
    inline DecenterData* decenter() const;
    inline SurfaceProfile* profile() const;
    inline Aperture* clear_aperture() const;
    inline Aperture* edge_aperture() const;
    inline Solve* solve() const;

    /** Return aperture shape name. If no aperture is set, returns "None" */
    std::string aperture_shape() const;

    /** Maximum size of the interface.
      * If an aperture is set, this function returns the aperture max dimension.
      * Otherwise, it returns valid semi-diameter.
      */
    double max_aperture() const;

    /** Maximum valid semi-diameter where the reference rays pass */
    inline double semi_diameter() const;

    /** Returns true if the given point(x,y) is inside of aperture */
    bool point_inside(double x, double y) const;
    bool point_inside(const Eigen::Vector2d& pt) const;

    inline const Transformation& local_transform() const;
    inline const Transformation& global_transform() const;

    template<SurfaceProfile::Type type>
    void set_profile(double cv, double k= 0.0, const std::vector<double>& coefs=std::vector<double>(10, 0.0));

    inline void set_label(std::string lbl);

    template<Aperture::Shape ap_shape>
    void set_clear_aperture(double x_dimension, double y_dimension=0.0);

    template<Aperture::Shape ap_shape>
    void set_edge_aperture(double x_dimension, double y_dimension=0.0);

    /** Remove all clear apertures from the surface */
    void remove_clear_aperture();

    inline void set_semi_diameter(double sd);

    inline void set_local_transform(const Transformation& tfrm);
    inline void set_global_transform(const Transformation& tfrm);

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

    /** Shape of the interface */
    std::unique_ptr<SurfaceProfile> profile_;

    std::unique_ptr<Aperture> edge_aperture_;
    std::unique_ptr<Aperture> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    std::unique_ptr<Solve> solve_;

    /** transform to next interface */
    Transformation lcl_tfrm_;

    /** global transform against the reference interface */
    Transformation gbl_tfrm_;
};



template<SurfaceProfile::Type type>
void Surface::set_profile(double cv, double k, const std::vector<double>& coefs)
{
    profile_.reset();

    switch (type) {
    case SurfaceProfile::Type::Sphere:
        profile_ = std::make_unique<Spherical>(cv);
        break;
    case SurfaceProfile::Type::EvenAsphere:
        profile_ = std::make_unique<EvenPolynomial>(cv,k,coefs);
        break;
    case SurfaceProfile::Type::OddAsphere:
        profile_ = std::make_unique<OddPolynomial>(cv,k,coefs);
        break;
    default:
        profile_ = std::make_unique<Spherical>(cv);
    }
}


std::string Surface::interact_mode() const
{
    return interact_mode_;
}

std::string Surface::label() const
{
    return label_;
}

DecenterData* Surface::decenter() const
{
    return decenter_.get();
}

SurfaceProfile* Surface::profile() const
{
    return profile_.get();
}

template <Aperture::Shape ap_shape>
void Surface::set_clear_aperture(double x_dimension, double /*y_dimension*/)
{
    clear_aperture_.reset();

    switch (ap_shape) {
    case Aperture::Shape::Circular:
        clear_aperture_ = std::make_unique<Circular>(x_dimension);
        break;
    case Aperture::Shape::Rectangular:
        // not implemented
        //break;
    default:
        clear_aperture_ = std::make_unique<Circular>(x_dimension);
    }
}

template <Aperture::Shape ap_shape>
void Surface::set_edge_aperture(double x_dimension, double /*y_dimension*/)
{
    edge_aperture_.reset();

    switch (ap_shape) {
    case Aperture::Shape::Circular:
        edge_aperture_ = std::make_unique<Circular>(x_dimension);
        break;
    case Aperture::Shape::Rectangular:
        // not implemented
        //break;
    default:
        edge_aperture_ = std::make_unique<Circular>(x_dimension);
    }
}

Aperture* Surface::clear_aperture() const
{
    return clear_aperture_.get();
}

Aperture* Surface::edge_aperture() const
{
    return edge_aperture_.get();
}

double Surface::semi_diameter() const
{
    return semi_diameter_;
}

const Transformation& Surface::local_transform() const
{
    return lcl_tfrm_;
}

const Transformation& Surface::global_transform() const
{
    return gbl_tfrm_;
}

Solve* Surface::solve() const
{
    return solve_.get();
}

void Surface::set_label(std::string lbl)
{
    label_ = lbl;
}

void Surface::set_local_transform(const Transformation& tfrm)
{
    lcl_tfrm_ = tfrm;
}

void Surface::set_global_transform(const Transformation& tfrm)
{
    gbl_tfrm_ = tfrm;
}

void Surface::set_semi_diameter(double sd)
{
    semi_diameter_ = sd;
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
