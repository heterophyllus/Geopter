#include <math.h>

#include "Material/air.h"

using namespace geopter;

Air::Air()
{
    name_ = "AIR";
    n_ = 1.0;
}

Air::~Air()
{

}

double Air::rindex(double wv_nm) const
{
    return 1.0;
}

double Air::refractive_index_abs(double wvl_micron, double T, double P)
{
    constexpr double P0 = 101325.0;
    constexpr double Tref = 15;
    double nref = refractive_index_reference(wvl_micron);
    double num = nref - 1.0;
    double denom = 1.0 + (T-Tref)*(3.4785*pow(10,-3));

    return ( 1.0 + (num/denom)*(P/P0) );
}

double Air::refractive_index_reference(double wvl_micron)
{
    double term1 = 6432.8;
    double term2 = 2949810.0*pow(wvl_micron, 2)/( 146.0*pow(wvl_micron,2) - 1.0 );
    double term3 = 25540.0*pow(wvl_micron,2)/( 41.0*pow(wvl_micron,2) - 1.0 );
    double nref = 1.0 + (term1 + term2 + term3)*pow(10, -8);

    return nref;
}
