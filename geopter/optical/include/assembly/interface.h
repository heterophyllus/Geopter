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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <memory>

#include "assembly/transformation.h"
#include "assembly/aperture.h"
#include "assembly/circular.h"
#include "assembly/decenter_data.h"

#include "profile/spherical.h"
#include "profile/even_polynomial.h"
#include "profile/odd_polynomial.h"

namespace geopter {

class Interface
{
public:
    Interface();
    virtual ~Interface();

    inline std::string interact_mode() const;
    inline std::string label() const;
    inline DecenterData* decenter() const;
    inline SurfaceProfile* profile() const;
    inline Aperture* clear_aperture() const;
    inline Aperture* edge_aperture() const;

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

    inline Transformation local_transform() const;
    inline Transformation global_transform() const;


    template<SurfaceProfile::Type type>
    void set_profile(double cv, double k= 0.0, const std::vector<double>& coefs=std::vector<double>(10, 0.0));

    void set_label(std::string lbl);

    template<Aperture::Shape ap_shape>
    void set_clear_aperture(double x_dimension, double y_dimension=0.0);

    template<Aperture::Shape ap_shape>
    void set_edge_aperture(double x_dimension, double y_dimension=0.0);

    /** Remove all clear apertures from the surface */
    void remove_clear_aperture();

    void set_semi_diameter(double sd);

    void set_local_transform(const Transformation& tfrm);
    void set_global_transform(const Transformation& tfrm);

    virtual void update();

protected:
    std::string label_;
    std::string interact_mode_;

    double semi_diameter_;

    /** Shape of the interface */
    std::unique_ptr<SurfaceProfile> profile_;

    std::unique_ptr<Aperture> edge_aperture_;
    std::unique_ptr<Aperture> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    /** transform to next interface */
    Transformation lcl_tfrm_;

    /** global transform against the reference interface */
    Transformation gbl_tfrm_;
};


template<SurfaceProfile::Type type>
void Interface::set_profile(double cv, double k, const std::vector<double>& coefs)
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


std::string Interface::interact_mode() const
{
    return interact_mode_;
}

std::string Interface::label() const
{
    return label_;
}

DecenterData* Interface::decenter() const
{
    return decenter_.get();
}

SurfaceProfile* Interface::profile() const
{
    return profile_.get();
}

template <Aperture::Shape ap_shape>
void Interface::set_clear_aperture(double x_dimension, double y_dimension)
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
void Interface::set_edge_aperture(double x_dimension, double y_dimension)
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

Aperture* Interface::clear_aperture() const
{
    return clear_aperture_.get();
}

Aperture* Interface::edge_aperture() const
{
    return edge_aperture_.get();
}

double Interface::semi_diameter() const
{
    return semi_diameter_;
}

Transformation Interface::local_transform() const
{
    return lcl_tfrm_;
}

Transformation Interface::global_transform() const
{
    return gbl_tfrm_;
}

} //namespace geopter

#endif // INTERFACE_H
