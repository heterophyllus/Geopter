#define _USE_MATH_DEFINES
#include <math.h>

#include "Spec/spectral_line.h"


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

