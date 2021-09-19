#include <vector>
#include <sstream>
#include <iomanip>

#include "spline.h"

#include "Analysis/longitudinal.h"

#include "Renderer/rgb.h"
#include "System/optical_system.h"
#include "Assembly/optical_assembly.h"
#include "Assembly/surface_profile.h"
#include "Sequential/ray.h"
#include "Sequential/sequential_trace.h"
#include "Spec/optical_spec.h"


using namespace geopter;

Longitudinal::Longitudinal(OpticalSystem* opt_sys, Renderer* renderer) :
    opt_sys_(opt_sys),
    renderer_(renderer),
    num_data_(10)
{

}

Longitudinal::~Longitudinal()
{
    opt_sys_ = nullptr;
    renderer_ = nullptr;
}

void Longitudinal::plot(double lsa_scale, double fie_scale)
{
    renderer_->set_grid_layout(1, 2); // LSA, FIE

    /* longitudinal spherical aberration */
    renderer_->set_current_cell(0,0);
    plot_lsa(lsa_scale);

    /* astigmatism */
    renderer_->set_current_cell(0,1);
    plot_ast(fie_scale);

    renderer_->update();
}


void Longitudinal::plot_lsa(double scale)
{
    /* longitudinal spherical aberration */
    //renderer_->set_current_cell(0,0);
    renderer_->set_x_axis_range(-scale, scale);
    renderer_->set_y_axis_range(0.0, 1.0);
    renderer_->draw_x_axis();
    renderer_->draw_y_axis();
    renderer_->set_x_axis_label("Spherical Aberration");
    renderer_->set_y_axis_label("Pupil Height");


    PupilCrd pupil;
    Ray ray;
    Field* fld0 = opt_sys_->optical_spec()->field_of_view()->field(0);

    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    std::vector<double> l_primes = get_l_primes();
    double img_dst = opt_sys_->first_order_data().img_dist;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        double dfoc = l_primes[wi] - l_primes[ref_wvl_idx];

        /* collect data */
        std::vector<double> py;
        std::vector<double> lsa;

        py.push_back(0.0);
        lsa.push_back(dfoc);

        std::vector<double> y_list;
        std::vector<double> N_list;
        std::vector<double> M_list;
        for(int ri = 1; ri < num_data_; ri++) {
            pupil(0) = 0.0;
            pupil(1) = (double)ri/(double)(num_data_-1);
            ray = tracer->trace_pupil_ray(pupil, fld0, wvl);

            //if(ray.status() == RayStatus::PassThrough){
                py.push_back(pupil(1));

                int at_image = ray.size()-1;
                double y = ray.y(at_image);
                double M = ray.M(at_image);
                double N = ray.N(at_image);
                lsa.push_back(-y*N/M);

                y_list.push_back(y);
                M_list.push_back(M);
                N_list.push_back(N);
            //}
        }

        /* interpolation */
        std::vector<double> spy(30);
        std::vector<double> slsa(30);

        tk::spline spline_lsa;
        spline_lsa.set_points(py, lsa);

        for(int i = 0; i < 30; i++) {
            spy[i] = (double)i/(30.0 - 1.0);
            slsa[i] = spline_lsa(spy[i]);
        }


        renderer_->draw_polyline(slsa, spy, color);
    }

    delete tracer;

}


void Longitudinal::plot_ast(double scale)
{
    double maxfld = opt_sys_->optical_spec()->field_of_view()->max_field();
    //renderer_->set_current_cell(0,1);
    renderer_->set_x_axis_range(-scale, scale);
    renderer_->set_y_axis_range(0.0, maxfld);
    renderer_->draw_x_axis();
    renderer_->draw_y_axis();
    renderer_->set_x_axis_label("z");
    renderer_->set_y_axis_label("Field");

    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    std::vector<double> l_primes = get_l_primes();
    double l_prime_ref = l_primes[ref_wvl_idx];

    Field *tmp_fld = new Field();

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvls; wi++){

        // collect data (10 points)
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> fy;
        std::vector<double> xfo;
        std::vector<double> yfo;

        double dfoc = l_primes[wi] - l_prime_ref;
        fy.push_back(0.0);
        xfo.push_back(dfoc);
        yfo.push_back(dfoc);
        for(int fi = 1; fi < num_data_; fi++) {
            double y = maxfld*(double)fi/(double)(num_data_-1);
            tmp_fld->set_y(y);
            PupilCrd aim_pt = tracer->aim_chief_ray(tmp_fld, wvl);
            tmp_fld->set_aim_pt(aim_pt);
            fy.push_back(y);

            Eigen::Vector2d s_t = tracer->trace_coddington(tmp_fld, wvl);
            xfo.push_back(s_t(0) + dfoc);
            yfo.push_back(s_t(1) + dfoc);
        }

        // spline interpolation
        tk::spline spline_xfo;
        tk::spline spline_yfo;
        spline_xfo.set_points(fy, xfo);
        spline_yfo.set_points(fy, yfo);

        std::vector<double> sfy(30);
        std::vector<double> sxfo(30), syfo(30);

        for (int i = 0; i < 30; i++) {
            sfy[i] = maxfld * (double)i / (30.0 - 1.0);
            sxfo[i] = spline_xfo(sfy[i]);
            syfo[i] = spline_yfo(sfy[i]);
        }

        renderer_->draw_polyline(sxfo, sfy, color, Renderer::LineStyle::Solid);
        renderer_->draw_polyline(syfo, sfy, color, Renderer::LineStyle::Dots);
    }

    delete tmp_fld;
    delete tracer;

}


std::vector<double> Longitudinal::get_l_primes()
{
    std::vector<double> l_primes;

    /*
    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys_->optical_spec()->spectral_region()->wavelength(wi);
        SequentialPath path = tracer->overall_sequential_path(wvl);

        double l = -path.at(0).d;
        double l_prime;
        double n_before = path.at(0).rind;
        double n_after;
        for(int i = 1; i < (int)path.size()-1; i++) {
            n_after = path.at(i).rind;

            double c = path.at(i).srf->profile()->cv();
            double d = path.at(i).rind;
            double phi = c*(n_after - n_before);

            l_prime = n_after/(phi + (n_before/l));
            n_before = n_after;
            l = l_prime - d;
        }

        l_primes.push_back(l_prime);
    }

    delete tracer;
    */
    int num_srf = opt_sys_->optical_assembly()->surface_count();
    int img_srf = num_srf-1;
    int last_surf = num_srf - 1 -1;
    int num_wvl = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    for(int wi = 0; wi < num_wvl; wi++){
        ParaxialRay ax_ray = opt_sys_->axial_ray(wi);
        double y = ax_ray.at(last_surf).ht;
        double u_prime = ax_ray.at(img_srf).slp;
        double l_prime = -y/u_prime;
        l_primes.push_back(l_prime);
    }

    return l_primes;
}
