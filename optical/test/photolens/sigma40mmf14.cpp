
#include <fstream>
#include <stdio.h>
#include <limits>

#include "optical_core.h"

using namespace geopter;

int main()
{
    /**
     *  Photographic lens design example.
     *
     *  The data used here was obtained from the patent document JPA2020-12952 (ex1).
     *  This optical system is known to be used for commercial product SIGMA 40mm F1.4 DG HSM|Art and its cinelens version 40mm T1.5.
     *
     */


    auto opm = std::make_shared<OpticalModel>();
    opm->set_name("SIGMA 40mm F1.4");

    /* setup optical specs */
    opm->optical_spec()->pupil_spec()->set_pupil_type(PupilType::FNO);
    opm->optical_spec()->pupil_spec()->set_value(1.45);

    opm->optical_spec()->field_of_view()->set_field_type(FieldType::OBJ_ANG);
    opm->optical_spec()->field_of_view()->add_field(0.0, 0.0,    1.0, rgb_black);
    opm->optical_spec()->field_of_view()->add_field(0.0, 27.935, 1.0, rgb_blue);

    opm->optical_spec()->spectral_region()->add(SpectralLine::C, 1.0, rgb_red);
    opm->optical_spec()->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);
    opm->optical_spec()->spectral_region()->add(SpectralLine::e, 1.0, rgb_green);
    opm->optical_spec()->spectral_region()->add(SpectralLine::F, 1.0, rgb_cyan);
    opm->optical_spec()->spectral_region()->add(SpectralLine::g, 1.0, rgb_blue);
    opm->optical_spec()->spectral_region()->set_reference_index(1); //d


    /* setup surface data */
    double d0  = 1.0e+10; //INF
    double d14 = 8.7665;
    double d19 = 7.3907;
    double BF  = 39.0002;

    // aspherical data
    double k = 0.0; //conic
    std::vector<double> coefs1 = { -2.73662e-6,
                                    3.07519e-9,
                                    3.90515e-11,
                                   -1.94154e-14};

    std::vector<double> coefs2 = {  3.26804e-6,
                                    3.98767e-9,
                                    3.58258e-11};

    opm->seq_model()->gap(0)->set_thi(d0);
    // group 1
    /*
    opm->seq_model()->append(  111.7037,  5.5478, std::make_shared<BuchdahlGlass>(1.77250, 49.62));
    opm->seq_model()->add_surface(  514.0093,  0.1500);
    opm->seq_model()->add_surface(  191.8963,  1.5000, std::make_shared<BuchdahlGlass>(1.43700, 95.10));
    opm->seq_model()->add_surface(   30.2492, 12.0599);
    opm->seq_model()->add_surface(  423.9805,  1.5000, std::make_shared<BuchdahlGlass>(1.43700, 95.10));
    opm->seq_model()->add_surface(   50.8216, 13.2984);
    opm->seq_model()->add_surface(  -38.1791,  1.5000, std::make_shared<BuchdahlGlass>(1.64769, 33.84));
    opm->seq_model()->add_surface(  172.5440,  9.3857, std::make_shared<BuchdahlGlass>(1.83481, 42.72));
    opm->seq_model()->add_surface(  -56.3040,  0.1500);
    opm->seq_model()->add_surface(-1641.6822, 12.4672, std::make_shared<BuchdahlGlass>(1.55032, 75.50));
    opm->seq_model()->add_surface(  -32.7021,  1.5000, std::make_shared<BuchdahlGlass>(1.60342, 38.01));
    opm->seq_model()->add_surface(  246.8799,  0.1500);
    opm->seq_model()->add_surface(   83.2754,  5.8820, std::make_shared<BuchdahlGlass>(1.77250, 49.62));
    opm->seq_model()->add_surface(  -377.0503,    d14);
    //group 2
    opm->seq_model()->add_surface(   90.4416,  5.3037, std::make_shared<BuchdahlGlass>(1.92286, 20.88));
    opm->seq_model()->add_surface( -527.4885,  0.1500);
    opm->seq_model()->add_surface(   55.6364, 11.8660, std::make_shared<BuchdahlGlass>(1.59282, 68.63));
    opm->seq_model()->add_surface(  -44.6058,  2.4820, std::make_shared<BuchdahlGlass>(1.58144, 40.89));
    opm->seq_model()->add_surface(   39.3845,     d19);
    //stop
    opm->seq_model()->add_surface(    0.0000,  1.5000);
    opm->seq_model()->set_stop();
    //group3
    opm->seq_model()->add_surface(   84.0231,  6.8887, std::make_shared<BuchdahlGlass>(1.43700, 95.10));
    opm->seq_model()->add_surface(  -32.2020,  1.5000, std::make_shared<BuchdahlGlass>(1.64769, 33.84));
    opm->seq_model()->add_surface(   66.2242,  7.8613);
    opm->seq_model()->add_surface(  -26.9856,  1.5000, std::make_shared<BuchdahlGlass>(1.62588, 35.74));
    opm->seq_model()->add_surface( -121.0290,  0.1500);
    opm->seq_model()->add_surface(   64.6922,  6.7685, std::make_shared<BuchdahlGlass>(1.59282, 68.63));
    opm->seq_model()->add_surface(  -40.8255,  0.1500);
    opm->seq_model()->add_surface( -430.0370,  4.6315, std::make_shared<BuchdahlGlass>(1.85135, 40.10));
    opm->seq_model()->add_surface(  -54.9865,      BF);

    opm->seq_model()->surface(28)->set_profile(std::make_unique<EvenPolynomial>(-1.0/430.0370, k, coefs1));
    opm->seq_model()->surface(29)->set_profile(std::make_unique<EvenPolynomial>( -1.0/54.9865, k, coefs2));
    */

    // group 1
    opm->seq_model()->append(  111.7037,  5.5478, "77250.4962");
    opm->seq_model()->append(  514.0093,  0.1500);
    opm->seq_model()->append(  191.8963,  1.5000, "43700.9510");
    opm->seq_model()->append(   30.2492, 12.0599);
    opm->seq_model()->append(  423.9805,  1.5000, "43700.9510");
    opm->seq_model()->append(   50.8216, 13.2984);
    opm->seq_model()->append(  -38.1791,  1.5000, "64769.3384");
    opm->seq_model()->append(  172.5440,  9.3857, "83481.4272");
    opm->seq_model()->append(  -56.3040,  0.1500);
    opm->seq_model()->append(-1641.6822, 12.4672, "55032.7550");
    opm->seq_model()->append(  -32.7021,  1.5000, "60342.3801");
    opm->seq_model()->append(  246.8799,  0.1500);
    opm->seq_model()->append(   83.2754,  5.8820, "77250.4962");
    opm->seq_model()->append(  -377.0503,    d14);
    //group 2
    opm->seq_model()->append(   90.4416,  5.3037, "92286.2088");
    opm->seq_model()->append( -527.4885,  0.1500);
    opm->seq_model()->append(   55.6364, 11.8660, "59282.6863");
    opm->seq_model()->append(  -44.6058,  2.4820, "58144.4089");
    opm->seq_model()->append(   39.3845,     d19);
    //stop
    opm->seq_model()->append(  std::numeric_limits<float>::infinity()  ,  1.5000);
    //opm->seq_model()->set_stop();
    //group3
    opm->seq_model()->append(   84.0231,  6.8887, "43700.9510");
    opm->seq_model()->append(  -32.2020,  1.5000, "64769.3384");
    opm->seq_model()->append(   66.2242,  7.8613);
    opm->seq_model()->append(  -26.9856,  1.5000, "62588.3574");
    opm->seq_model()->append( -121.0290,  0.1500);
    opm->seq_model()->append(   64.6922,  6.7685, "59282.6863");
    opm->seq_model()->append(  -40.8255,  0.1500);
    opm->seq_model()->append( -430.0370,  4.6315, "85135.4010");
    opm->seq_model()->append(  -54.9865,      BF);

    opm->seq_model()->surface(28)->set_profile(std::make_unique<EvenPolynomial>(-1.0/430.0370, k, coefs1));
    opm->seq_model()->surface(29)->set_profile(std::make_unique<EvenPolynomial>( -1.0/54.9865, k, coefs2));

    opm->seq_model()->set_stop(20);

    // set ray cutting aperture
    opm->seq_model()->surface(9)->set_clear_aperture(std::make_unique<Circular>(24.0));
    opm->seq_model()->surface(29)->set_clear_aperture(std::make_unique<Circular>(15.8));
    opm->update_model();
    opm->seq_model()->compute_vignetting();

    opm->update_model();

    opm->seq_model()->compute_vignetting();
    opm->update_model();


    /* print data */
    std::ostringstream oss;

    oss << "Title: " << opm->name() << std::endl;

    oss << "OpticalSpecs:" << std::endl;
    opm->optical_spec()->field_of_view()->print(oss);
    opm->optical_spec()->spectral_region()->print(oss);

    oss << "Sequential Model:" << std::endl;
    opm->seq_model()->print(oss);

    oss << "First Order Data:" << std::endl;
    opm->paraxial_model()->first_order_data().print(oss);

    oss << "Paraxial Rays:" << std::endl;
    oss << "Axial Ray:" << std::endl;
    opm->paraxial_model()->ax().print(oss);
    oss << "Principle Ray:" << std::endl;
    opm->paraxial_model()->pr().print(oss);


    std::ofstream fout;
    fout.open("SIGMA40mmF14.txt", std::ios::out);
    fout << oss.str() << std::endl;
    fout.close();

    std::cout << oss.str() << std::endl;


    /* draw layout */
    std::cout << "layout..." << std::endl;
    {
    RendererSvg *renderer = new RendererSvg("layout.svg", 500, 500);
    renderer->set_x_axis_range(-20, 180);
    renderer->set_y_axis_range(-100,100);

    Layout layout(opm.get(), renderer);
    layout.draw_elements();
    layout.draw_reference_rays();
    delete renderer;
    }

    /* ray fan */
    RendererSvg *renderer = new RendererSvg("ray_fan.svg", 800, 800);
    RayFan rayfan(opm.get(),renderer);
    rayfan.plot(0.5);
    delete renderer;

    /* save */
    FileIO::save_to_json(*opm, "photolens.json");


    std::cout << "----- End -----" << std::endl;

}
