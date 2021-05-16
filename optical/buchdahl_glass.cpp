#define _USE_MATH_DEFINES
#include <math.h>

#include "buchdahl_glass.h"
#include "spectral_line.h"


using namespace geopter;

BuchdahlGlass::BuchdahlGlass(double nd, double vd) : Medium(),
    wv0_(SpectralLine::d/1000.0),
    rind0_(nd),
    vd_(vd)
{
    n_ = nd;
    compute_coefs();
    display_name_ = glass_code();
}

BuchdahlGlass::~BuchdahlGlass()
{

}


double BuchdahlGlass::rindex(double wv_nm) const
{
    double om = omega(wv_nm/1000.0 - wv0_);
    return rind0_ + v1_*om + v2_*pow(om,2);
}

std::string BuchdahlGlass::glass_code()
{
    std::string nd_part = std::to_string( (long int)round((n_-1.0)*10000.0) );
    std::string vd_part = std::to_string( (int)round(vd_*100.0) );
    return nd_part + "." + vd_part;
}

double BuchdahlGlass::abbe_d() const
{
    return vd_;
}

void BuchdahlGlass::compute_coefs()
{
    //compute v1, v2
    const double b = -0.064667;
    const double m = -1.604048;
    const double wF = SpectralLine::F / 1000.0;
    const double wC = SpectralLine::C / 1000.0;

    double omF = omega(wF - wv0_);
    double omC = omega(wC - wv0_);
    double delta_om  = omF - omC;
    double delta_om2 = omF*omF - omC*omC;

    double dFC = (n_-1.0)/vd_;
    v2_ = ( dFC - b*delta_om ) / ( m*delta_om - delta_om2 );
    v1_ = b + m*v2_;
}

double BuchdahlGlass::omega(double delta_lambda) const
{
    return delta_lambda/( 1 + 2.5*delta_lambda );
}
