
#include <fstream>
#include <stdio.h>

#include "optical_core.h"

using namespace geopter;

int main()
{
    /*
     * This example shows simplest lens configuration.
     *
     * */


    std::cout << " ----- Triplet start -----" << std::endl;

    std::ofstream fout;
    std::ostringstream oss;

    auto opm = std::make_shared<OpticalModel>();
    opm->set_name("Sasian Triplet 50/4");
    oss << opm->name() << std::endl;

    oss << "Setting OpticalSpecs...";
    opm->optical_spec()->pupil_spec()->set_pupil_type(PupilType::EPD);
    opm->optical_spec()->pupil_spec()->set_value(12.5);
    opm->optical_spec()->pupil_spec()->print(oss);

    opm->optical_spec()->field_of_view()->set_field_type(FieldType::OBJ_ANG);
    opm->optical_spec()->field_of_view()->add_field(0.0, 0.0,  1.0, rgb_black);
    opm->optical_spec()->field_of_view()->add_field(0.0, 14.0, 1.0, rgb_red);
    opm->optical_spec()->field_of_view()->add_field(0.0, 20.0, 1.0, rgb_blue);
    opm->optical_spec()->field_of_view()->print(oss);

    opm->optical_spec()->spectral_region()->add(SpectralLine::C, 1.0, rgb_red);
    opm->optical_spec()->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);
    opm->optical_spec()->spectral_region()->add(SpectralLine::F, 1.0, rgb_cyan);
    opm->optical_spec()->spectral_region()->set_reference_index(1);
    opm->optical_spec()->spectral_region()->print(oss);

    oss << std::endl;
    std::cout << "done" << std::endl;

    // create model
    std::cout << "creating model";
    opm->seq_model()->gap(0)->set_thi(1.0e+10);

    opm->seq_model()->append(std::make_unique<Spherical>(1.0/23.7130),    4.831, std::make_shared<BuchdahlGlass>(1.691, 54.71));
    opm->seq_model()->append(std::make_unique<Spherical>(1.0/7331.2880),  5.860);
    opm->seq_model()->append(std::make_unique<Spherical>(-1.0/24.4560),   0.975, std::make_shared<BuchdahlGlass>(1.672, 32.25));
    opm->seq_model()->append(std::make_unique<Spherical>(1.0/21.8960),    4.822);
    opm->seq_model()->append(std::make_unique<Spherical>(1.0/86.7590),    3.127, std::make_shared<BuchdahlGlass>(1.691, 54.71));
    opm->seq_model()->append(std::make_unique<Spherical>(-1.0/20.4942),  41.2365);

    opm->seq_model()->set_stop(3);

    opm->update_model();

    std::cout << "done" << std::endl;

    oss << "Sequential Model:" << std::endl;
    opm->seq_model()->print(oss);
    oss << std::endl;

    // first order
    oss << "First Order Data:" << std::endl;
    opm->paraxial_model()->first_order_data().print(oss);
    oss << std::endl;


    // paraxial rays
    oss << "Paraxial Rays :" << std::endl;
    oss << "Axial Ray :" << std::endl;
    opm->paraxial_model()->ax().print(oss);

    oss << "Principle Ray :" << std::endl;
    opm->paraxial_model()->pr().print(oss);


    // output text
    fout.open("triplet.txt", std::ios::out);
    fout << oss.str() << std::endl;
    fout.close();

    // layout
    RendererSvg *renderer = new RendererSvg("triplet_layout.svg", 400, 300);
    renderer->set_x_axis_range(-10, 100);
    renderer->set_y_axis_range(-30,30);
    Layout layout(opm.get(), renderer);
    layout.draw_elements();
    layout.draw_reference_rays();

    delete renderer;


    // save to json
    FileIO::save_to_json(*opm, "triplet.json");

    std::cout << "----- Triplet End -----" << std::endl;

    opm.reset();
}
