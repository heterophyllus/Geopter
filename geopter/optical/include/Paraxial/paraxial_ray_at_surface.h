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

//============================================================================
/// \file   paraxial_ray_at_surface.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef PARAXIALRAYATSURFACE_H
#define PARAXIALRAYATSURFACE_H

namespace geopter {


/** Paraxial ray parameter on each surface */
class ParaxialRayAtSurface
{
public:
    ParaxialRayAtSurface(){
        y_ = 0.0;
        u_prime_ = 0.0;
        i_ = 0.0;
        n_prime_ = 0.0;
        before_ = nullptr;
    }

    ParaxialRayAtSurface(double y, double u_prime, double i, double n_prime, ParaxialRayAtSurface *before = nullptr){
        y_ = y;
        u_prime_ = u_prime;
        i_ = i;
        n_prime_ = n_prime;
        before_ = before;
    }

    ~ParaxialRayAtSurface(){
        before_ = nullptr;
    }

    void set_before(ParaxialRayAtSurface* before){
        before_ = before;
    }

    inline double y() const;
    inline double u() const;
    inline double u_prime() const;
    inline double i() const;
    inline double n() const;
    inline double n_prime() const;

    inline double l_prime() const;

private:
    double y_;
    double u_prime_; // outgoing
    double i_;       // incident
    double n_prime_; // outgoing

    ParaxialRayAtSurface *before_;
};


double ParaxialRayAtSurface::y() const
{
    return y_;
}

double ParaxialRayAtSurface::u() const
{
    if(before_){
        return before_->u_prime();
    }else{
        return u_prime_;
    }
}

double ParaxialRayAtSurface::u_prime() const
{
    return u_prime_;
}

double ParaxialRayAtSurface::i() const
{
    return i_;
}

double ParaxialRayAtSurface::n() const
{
    if(before_){
        return before_->n_prime();
    }else{
        return n_prime_;
    }
}

double ParaxialRayAtSurface::n_prime() const
{
    return n_prime_;
}

double ParaxialRayAtSurface::l_prime() const
{
    return (-y_/u_prime_);
}

}

#endif
