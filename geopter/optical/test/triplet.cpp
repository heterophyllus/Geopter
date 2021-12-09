#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <limits>

#include "optical.h"

using namespace geopter;

int main()
{

    /**************************
     * Set up OpticalSystem
     * *************************/
    std::cout << "Set up Opticalsystem..." << std::endl;

    // create optical system
    OpticalSystem *sys = new OpticalSystem;
    sys->set_title("Sasian Triplet 50mm F4");
    sys->set_note("This is a simple example");

    // set up  optical specifications
    // pupil
    auto opt_spec = sys->optical_spec();
    opt_spec->pupil_spec()->set_pupil_type(PupilType::EPD);
    opt_spec->pupil_spec()->set_value(12.5);

    // fields
    opt_spec->field_of_view()->set_field_type(FieldType::OBJ_ANG);
    opt_spec->field_of_view()->add(0.0,  0.0, 1.0, rgb_black);
    opt_spec->field_of_view()->add(0.0, 14.0, 1.0, rgb_red);
    opt_spec->field_of_view()->add(0.0, 20.0, 1.0, rgb_blue);

    // wavelengths
    opt_spec->spectral_region()->add(SpectralLine::C, 1.0, rgb_red);
    opt_spec->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);
    opt_spec->spectral_region()->add(SpectralLine::F, 1.0, rgb_cyan);
    opt_spec->spectral_region()->set_reference_index(1);
    const double ref_wvl_val = opt_spec->spectral_region()->reference_wvl(); // reference wavelength

    // set up lens data
    auto assembly = sys->optical_assembly();

    assembly->setup_from_text("1,   23.7130,  4.8310, 1.69100:54.71",
                              "2, 7331.2880,  5.8600",
                              "3,  -24.4560,  0.9750, 1.67271:32.25",
                              "4,   21.8960,  4.8220",
                              "5,   86.7590,  3.1270, 1.69100:54.71",
                              "6,  -20.4942, 41.2365" );
    // set object distance
    assembly->gap(0)->set_thi(1.0e+10);
    assembly->set_stop(3);

    // update
    sys->update_model();

    // print system data
    std::ostringstream oss;
    sys->print(oss);

    std::ofstream fout;
    fout.open("triplet.txt", std::ios::out);
    fout << oss.str() << std::endl;
    fout.close();

    // save optical system to json
    //sys->save_to_file("triplet.json");


    /***************************
     * Analysis
     * *************************/
    std::cout << "Analysis..." << std::endl;

    // 2d layout
    /*
    std::cout << "Layout..." ;
    RendererSvg svg;
    Layout layout(sys, &svg);
    layout.draw_elements();
    layout.draw_reference_rays();
    svg.savefig("layout.svg");
    std::cout << "done" << std::endl;
    */




    // transverse ray fan
    std::cout << "Transverse Ray Fan...";
    const int num_fld = sys->optical_spec()->field_of_view()->field_count();

    RendererGnuplot renderer;
    renderer.set_grid_layout(num_fld, 1);

    TransverseRayFan rayfan(sys);

    for(int fi = 0; fi < num_fld; fi++){
        Field* fld = sys->optical_spec()->field_of_view()->field(fi);
        auto plot_data = rayfan.plot(21, fld);
        renderer.set_current_cell(num_fld - fi - 1, 0); // bottom to top
        renderer.plot(plot_data);
    }

    renderer.show();
    std::cout << "done" << std::endl;


/*
    std::cout << "Spot Diagram...";
    renderer.set_grid_layout(num_fld, 1);

    SpotDiagram spot(sys);
    for(int fi = 0; fi < num_fld; fi++){
        Field* fld = sys->optical_spec()->field_of_view()->field(fi);
        auto plot_data = spot.plot(fld, SpotDiagram::Grid, 21, 1.0);
        plot_data->set_plot_style(Renderer::PlotStyle::Curve);
        renderer.set_current_cell(num_fld - fi - 1, 0); // bottom to top
        renderer.plot(plot_data);
    }
    renderer.show();
    std::cout << "done" << std::endl;


    // Wavefront
    std::cout << "Wavefront...";

    Field* fld = sys->optical_spec()->field_of_view()->field(0);
    Wavefront wavefront(sys);
    wavefront.from_opd_trace(sys, fld, ref_wvl_val, 128);
    renderer.imshow(wavefront.to_matrix());
    renderer.show();
    std::cout << "done" << std::endl;
*/
    delete sys;

}
