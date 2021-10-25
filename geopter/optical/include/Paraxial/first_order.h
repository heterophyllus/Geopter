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
/// \file   first_order.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef FIRSTORDERDATA_H
#define FIRSTORDERDATA_H

#include <sstream>

namespace geopter {

/** First order data container */
struct FirstOrderData{
    FirstOrderData() {
        opt_inv = 1.0;
        efl = 0.0;
        ffl = 0.0;
        bfl = 0.0;
        fno = 0.0;
        img_dist = 0.0;
        obj_ang = 0.0;
        
    }

    /** optical invariant */
    double opt_inv;

    /** effective focla length */
    double efl;

    /** front focas */
    double ffl;

    /** back focus */
    double bfl;

    /** F number */
    double fno;

    

    /** actual image distance */
    double img_dist;

    /** maximum incident angle in objective space */
    double obj_ang;

    /** paraxial image height */
    double img_ht;

    double img_na;

    /** entrance pupil distance (from s1) */
    double enp_dist;

    /** entrance pupil radius */
    double enp_radius;

    /** exit pupil distance (from sI-1) */
    double exp_dist;

    /** exit pupil radius */
    double exp_radius;


    double n_obj;

    double n_img;

    /** magnification */
    double m;

    /** reduction rate */
    double red;

    /** object distance */
    double obj_dist;

    double obj_na;

    double pp1;

    double ppk;

    void print();
    void print(std::ostringstream& oss);
};

}

#endif // FIRSTORDERDATA_H
