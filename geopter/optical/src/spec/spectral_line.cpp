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

#include "spec/spectral_line.h"


using namespace geopter;

//http://www.hoya-opticalworld.com/japanese/technical/002.html
const double SpectralLine::t = 1013.980;
const double SpectralLine::s =  852.110;
const double SpectralLine::r =  706.519;
const double SpectralLine::C =  656.273;
const double SpectralLine::C_=  643.847;
const double SpectralLine::D =  589.294;
const double SpectralLine::d =  587.562;
const double SpectralLine::e =  546.074;
const double SpectralLine::F =  486.133;
const double SpectralLine::F_=  479.991;
const double SpectralLine::g =  435.834;
const double SpectralLine::h =  404.656;
const double SpectralLine::i =  365.015;


double SpectralLine::wavelength(const std::string& spectralname)
{
    if(spectralname == "t"){
        return SpectralLine::t;
    }
    else if(spectralname == "s"){
        return SpectralLine::s;
    }
    else if(spectralname == "r"){
        return SpectralLine::r;
    }
    else if(spectralname == "C"){
        return SpectralLine::C;
    }
    else if(spectralname == "C_"){
        return SpectralLine::C_;
    }
    else if(spectralname == "D"){
        return SpectralLine::D;
    }
    else if(spectralname == "d"){
        return SpectralLine::d;
    }
    else if(spectralname == "e"){
        return SpectralLine::e;
    }
    else if(spectralname == "F"){
        return SpectralLine::F;
    }
    else if(spectralname == "F_"){
        return SpectralLine::F_;
    }
    else if(spectralname == "g"){
        return SpectralLine::g;
    }
    else if(spectralname == "h"){
        return SpectralLine::h;
    }
    else if(spectralname == "i"){
        return SpectralLine::i;
    }
    else{
        return NAN;
    }
}

