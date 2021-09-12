#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <limits>

#include "optical.h"

using namespace geopter;

int main()
{
    /* triplet */

    std::cout << "=== triplet ===" << std::endl;

    std::ostringstream oss;

    auto sys = std::make_shared<OpticalSystem>();
    sys->set_title("Sasian Triplet 50mm F4");


    /*
     set up optical specifications
     */
    
    /* Defines system aperture
       Entrance Pupil Diameter : 25.0   
    */
    auto opt_spec = sys->optical_spec();
    opt_spec->pupil_spec()->set_pupil_type(PupilType::EPD);
    opt_spec->pupil_spec()->set_value(12.5);

    /* Defines field of view
       Object Angle : 0.0, 14.0, 20.0 (deg)
    */
    opt_spec->field_of_view()->set_field_type(FieldType::OBJ_ANG);
    opt_spec->field_of_view()->add(0.0,  0.0, 1.0, rgb_black);
    opt_spec->field_of_view()->add(0.0, 14.0, 1.0, rgb_red);
    opt_spec->field_of_view()->add(0.0, 20.0, 1.0, rgb_blue);

    /* Defines spectral region using Fraunhofer spectral line
       C, d, F
       The weights of all wavelengths are set to 1.0
       Rendering color used for any plots are set to red, black, green
       Set "d" as reference wavelength
    */
    opt_spec->spectral_region()->add(SpectralLine::C, 1.0, rgb_red);
    opt_spec->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);
    opt_spec->spectral_region()->add(SpectralLine::F, 1.0, rgb_cyan);
    opt_spec->spectral_region()->set_reference_index(1);
    const int ref_wi = 1; // d


    /*
     Set up optical system
     
     Optical system is sequential construction of optical interfaces and gaps filled with optical material.
    */
    double inf = std::numeric_limits<double>::infinity();
    auto assembly = sys->optical_assembly();
    //assembly->set_object_distance(1.0e+10);
    assembly->add_surface_and_gap(       inf, 1.0e+10, "AIR"); // Obj
    assembly->add_surface_and_gap(   23.7130,  4.8310, "1.69100:54.71");
    assembly->add_surface_and_gap( 7331.2880,  5.8600, "AIR");
    assembly->add_surface_and_gap(  -24.4560,  0.9750, "1.67271:32.25");
    assembly->add_surface_and_gap(   21.8960,  4.8220, "AIR");
    assembly->add_surface_and_gap(   86.7590,  3.1270, "1.69100:54.71");
    assembly->add_surface_and_gap(  -20.4942, 41.2365, "AIR");
    assembly->add_surface_and_gap(       inf,     0.0, "AIR"); // Img
    assembly->set_stop(3);

    sys->update_model();

    assembly->print(oss);


    /*
     Accessing paraxial data

     After updating optical system, paraxial data have been computed.
    */
    auto ax_ray = sys->parax_data()->axial_ray(ref_wi);
    auto pr_ray = sys->parax_data()->principle_ray(ref_wi);
    auto fod = sys->parax_data()->first_order_data();

    ax_ray.print(oss);
    pr_ray.print(oss);
    fod.print(oss);


    /* reference ray
    Reference rays are frequently used for various purpose.
    
    */
    oss << "Upper marginal ray at F0" << std::endl;
    Ray ray = sys->sequential_data()->reference_ray(2, 0, ref_wi);
    ray.print(oss);

    oss << "Upper marginal ray at F2" << std::endl;
    ray = sys->sequential_data()->reference_ray(2, 2, ref_wi);
    ray.print(oss);

    oss << "Lower marginal ray at F2" << std::endl;
    ray = sys->sequential_data()->reference_ray(3, 2, ref_wi);
    ray.print(oss);

    // output to text
    std::ofstream fout;
    fout.open("triplet.txt", std::ios::out);
    fout << oss.str() << std::endl;
    fout.close();

    // print
    std::cout << oss.str() << std::endl;

    // save to json
    FileIO::save_to_json(*sys, "triplet.json");

    sys.reset();

    std::cout << "=== Finish ===" << std::endl;
}
