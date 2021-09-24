#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>

#include "spline.h"

#include "Analysis/aberration.h"

#include "Renderer/rgb.h"
#include "System/optical_system.h"
#include "Spec/optical_spec.h"
#include "Assembly/optical_assembly.h"
#include "Assembly/surface_profile.h"
#include "Sequential/ray.h"
#include "Sequential/sequential_trace.h"
#include "Paraxial/paraxial_trace.h"
#include "Paraxial/paraxial_ray.h"

using namespace geopter;

Aberration::Aberration(OpticalSystem* opt_sys, Renderer* renderer) :
    opt_sys_(opt_sys),
    renderer_(renderer)
{
    num_fld_ = opt_sys_->optical_spec()->field_of_view()->field_count();
    num_wvl_ = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    ref_wvl_val_ = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    ref_wvl_idx_ = opt_sys_->optical_spec()->spectral_region()->reference_index();
}

Aberration::~Aberration()
{
    opt_sys_ = nullptr;
    renderer_ = nullptr;
}

void Aberration::plot_transverse_aberration(double scale, double nrd)
{
    constexpr int num_interpolation = 50;
    const int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    renderer_->set_grid_layout(num_flds, 2);

    Eigen::Vector2d pupil;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    for(int fi = 0; fi < num_flds; fi++)
    {
        // trace chief ray
        //Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);

        Ray chief_ray = opt_sys_->reference_ray(1,fi,ref_wvl_idx_);
        double x0 = chief_ray.back().x();
        double y0 = chief_ray.back().y();

        // trace zonal rays for all wavelengths
        for(int wi = 0; wi < num_wvls; wi++)
        {
            Rgb render_color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

            std::vector<double> px, py;
            std::vector<double> dx, dy;
            /*
            px.reserve(nrd);
            py.reserve(nrd);
            dx.reserve(nrd);
            dy.reserve(nrd);
            */

            Ray tangential_ray;
            Ray sagittal_ray;

            for(int ri = 0; ri < nrd; ri++)
            {
                // tangential
                pupil(0) = 0.0;
                pupil(1) = -1.0 + (double)ri*2.0/(double)(nrd-1);

                tangential_ray = tracer->trace_pupil_ray(pupil, fi, wi);

                if(tangential_ray.status() == RayStatus::PassThrough){
                    double y = tangential_ray.back().y();
                    py.push_back(pupil(1));
                    dy.push_back(y - y0);
                }

                // sagittal
                pupil(0) = -1.0 + (double)ri*2.0/(double)(nrd-1);
                pupil(1) = 0.0;

                sagittal_ray = tracer->trace_pupil_ray(pupil, fi, wi);
                if(sagittal_ray.status() == RayStatus::PassThrough){
                    double x = sagittal_ray.back().x();
                    px.push_back(pupil(0));
                    dx.push_back(x - x0);
                }
            }

            // interpolation
            tk::spline spline_tangential;
            spline_tangential.set_points(py,dy);

            tk::spline spline_sagittal;
            spline_sagittal.set_points(px,dx);

            std::vector<double> spx(num_interpolation), spy(num_interpolation);
            std::vector<double> sdx(num_interpolation), sdy(num_interpolation);
            double px_min = *std::min_element(px.begin(), px.end());
            double px_max = *std::max_element(px.begin(), px.end());
            double py_min = *std::min_element(py.begin(), py.end());
            double py_max = *std::max_element(py.begin(), py.end());
            for(int k = 0; k < num_interpolation; k++){
                spx[k] = px_min + (double)k*(px_max-px_min)/(double)(num_interpolation -1);
                spy[k] = py_min + (double)k*(py_max-py_min)/(double)(num_interpolation -1);
                sdx[k] = spline_sagittal(spx[k]);
                sdy[k] = spline_tangential(spy[k]);
            }

            // draw
            renderer_->set_current_cell(num_flds - fi -1, 0);
            renderer_->set_y_axis_range(-scale, scale);
            renderer_->set_x_axis_range(-1.0, 1.0);
            renderer_->set_y_axis_label("dy");
            renderer_->draw_polyline(spy, sdy, render_color);
            renderer_->draw_x_axis();
            renderer_->draw_y_axis();

            renderer_->set_current_cell(num_flds - fi -1, 1);
            renderer_->set_y_axis_range(-scale, scale);
            renderer_->set_x_axis_range(-1.0, 1.0);
            renderer_->set_y_axis_label("dx");
            renderer_->draw_polyline(spx, sdx, render_color);
            renderer_->draw_x_axis();
            renderer_->draw_y_axis();
        }

    }

    delete tracer;

    renderer_->update();
}

void Aberration::plot_longitudinal_spherical_aberration(double scale)
{
    constexpr int num_data = 10;

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
    double img_dst = opt_sys_->first_order_data().img_dist;

    // collect l_prime
    std::vector<double> l_primes;
    {
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
    }


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
        for(int ri = 1; ri < num_data; ri++) {
            pupil(0) = 0.0;
            pupil(1) = (double)ri/(double)(num_data-1);
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

void Aberration::plot_astigmatism(double scale)
{
    constexpr int num_data = 10;

    double maxfld = opt_sys_->optical_spec()->field_of_view()->max_field();
    //renderer_->set_current_cell(0,1);
    renderer_->set_x_axis_range(-scale, scale);
    renderer_->set_y_axis_range(0.0, maxfld);
    renderer_->draw_x_axis();
    renderer_->draw_y_axis();
    renderer_->set_x_axis_label("Astigmatism");
    renderer_->set_y_axis_label("Field");

    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    std::vector<double> l_primes;
    {
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
    }
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
        for(int fi = 1; fi < num_data; fi++) {
            double y = maxfld*(double)fi/(double)(num_data-1);
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

void Aberration::plot_chromatic_focus_shift(double lower_wvl, double higher_wvl)
{
    constexpr int num_data = 100;

    const int num_srf = opt_sys_->optical_assembly()->surface_count();
    const int img_srf = num_srf-1;
    const int last_surf = num_srf - 1 -1;


    double ref_wvl  = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    double wvl_step = (higher_wvl-lower_wvl)/(num_data-1);

    double u0, y0, ubar0, ybar0;

    ParaxialTrace *tracer = new ParaxialTrace(opt_sys_);
    tracer->get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    ParaxialRay ref_prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, ref_wvl);
    double y = ref_prx_ray.at(last_surf).ht;
    double u_prime = ref_prx_ray.at(img_srf).slp;
    double ref_l_prime = -y/u_prime;


    std::vector<double> xdata, ydata;

    for(int i = 0; i < num_data; i++) {
        double wvl = lower_wvl + i*wvl_step;
        ParaxialRay prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);

        y = prx_ray.at(last_surf).ht;
        u_prime = prx_ray.at(img_srf).slp;
        double l_prime = -y/u_prime;
        ydata.push_back(l_prime - ref_l_prime);
        xdata.push_back(wvl);
    }

    delete tracer;

    renderer_->draw_polyline(xdata, ydata, rgb_black, Renderer::LineStyle::Solid);

    double y_scale_higer = *std::max_element(ydata.begin(), ydata.end());
    double y_scale_lower = *std::min_element(ydata.begin(), ydata.end());

    renderer_->set_x_axis_range(lower_wvl, higher_wvl);
    renderer_->set_y_axis_range(y_scale_lower, y_scale_higer);
    renderer_->draw_x_axis();
    renderer_->draw_y_axis();
    renderer_->set_x_axis_label("Wavelength(nm)");
    renderer_->set_y_axis_label("Focus Shift");
    renderer_->update();
}


void Aberration::plot_spot_diagram(int nrd, double scale, double dot_size)
{
    renderer_->set_grid_layout(num_fld_,1);

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    Eigen::Vector2d pupil;
    std::vector<double> ray_dx, ray_dy;

    for(int fi = 0; fi < num_fld_; fi++){
        renderer_->set_current_cell(num_fld_ - fi -1, 0);

        // trace chief ray
        //pupil << 0.0, 0.0;
        //Ray chief_ray = tracer->trace_pupil_ray(pupil, fi, ref_wvl_idx_);
        Ray chief_ray = opt_sys_->reference_ray(1, fi, ref_wvl_idx_);
        double chief_ray_x = chief_ray.back().x();
        double chief_ray_y = chief_ray.back().y();

        for(int wi = 0; wi < num_wvl_; wi++){
            ray_dx.clear();
            ray_dy.clear();

            for(int pi = 0; pi < nrd; pi++) {
                for(int pj = 0; pj < nrd; pj++) {
                    pupil(0) = -1.0 + (double)pi*2.0/(double)(nrd-1);
                    pupil(1) = -1.0 + (double)pj*2.0/(double)(nrd-1);

                    Ray ray = tracer->trace_pupil_ray(pupil,fi,wi);

                    if(ray.status() == RayStatus::PassThrough){
                        ray_dx.push_back(ray.back().x() - chief_ray_x);
                        ray_dy.push_back(ray.back().y() - chief_ray_y);

                    }
                }
            }
            Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();
            renderer_->draw_dots(ray_dx, ray_dy, color, dot_size);

        }

        renderer_->set_y_axis_range(-scale, scale);
        renderer_->set_x_axis_range(-scale, scale);
        renderer_->set_x_axis_label("x");
        renderer_->set_y_axis_label("y");
        renderer_->draw_x_axis();
        renderer_->draw_y_axis();
        renderer_->set_aspect_ratio(1.0);
    }

    delete tracer;

    renderer_->update();

}
