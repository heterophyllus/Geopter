#define _USE_MATH_DEFINES
#include <math.h>

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
    renderer_->set_grid_layout(num_fld_, 2);

    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    const double stop_radius = opt_sys_->optical_assembly()->surface(stop_index)->max_aperture();

    Eigen::Vector2d pupil;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(false);
    tracer->set_apply_vig(true);

    for(int fi = 0; fi < num_fld_; fi++)
    {
        // trace chief ray
        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);

        std::shared_ptr<Ray> chief_ray = opt_sys_->reference_ray(1,fi,ref_wvl_idx_);
        double x0 = chief_ray->back()->x();
        double y0 = chief_ray->back()->y();

        // trace zonal rays for all wavelengths
        for(int wi = 0; wi < num_wvl_; wi++)
        {
            double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
            Rgb render_color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

            std::vector<double> px, py;
            std::vector<double> dx, dy;

            px.reserve(nrd);
            py.reserve(nrd);
            dx.reserve(nrd);
            dy.reserve(nrd);

            for(int ri = 0; ri < nrd; ri++)
            {
                // tangential
                pupil(0) = 0.0;
                pupil(1) = -1.0 + (double)ri*2.0/(double)(nrd-1);

                auto tangential_ray = tracer->trace_pupil_ray(pupil, fld, wvl);
                if(tangential_ray->status() == RayStatus::PassThrough){
                    double y = tangential_ray->back()->y();
                    Eigen::Vector2d vig_pupil = fld->apply_vignetting(pupil);
                    //py.push_back(vig_pupil(1));
                    double y_at_stop = tangential_ray->at(stop_index)->y();
                    py.push_back(y_at_stop);
                    dy.push_back(y - y0);
                }

                // sagittal
                pupil(0) = -1.0 + (double)ri*2.0/(double)(nrd-1);
                pupil(1) = 0.0;

                auto sagittal_ray = tracer->trace_pupil_ray(pupil, fld, wvl);
                if(sagittal_ray->status() == RayStatus::PassThrough){
                    double x = sagittal_ray->back()->x();
                    Eigen::Vector2d vig_pupil = fld->apply_vignetting(pupil);
                    //px.push_back(vig_pupil(0));
                    double x_at_stop = sagittal_ray->at(stop_index)->x();
                    px.push_back(x_at_stop);
                    dx.push_back(x - x0);
                }
            }


            // draw
            double h_scale = stop_radius;
            renderer_->set_current_cell(num_fld_ - fi -1, 0);
            renderer_->set_y_axis_range(-scale, scale);
            renderer_->set_x_axis_range(-h_scale, h_scale);
            renderer_->set_y_axis_label("dy");
            //renderer_->draw_polyline(spy, sdy, render_color);
            renderer_->draw_polyline(py, dy, render_color);
            renderer_->draw_x_axis();
            renderer_->draw_y_axis();

            renderer_->set_current_cell(num_fld_ - fi -1, 1);
            renderer_->set_y_axis_range(-scale, scale);
            renderer_->set_x_axis_range(-h_scale, h_scale);
            renderer_->set_y_axis_label("dx");
            //renderer_->draw_polyline(spx, sdx, render_color);
            renderer_->draw_polyline(px, dx, render_color);
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
    std::shared_ptr<Ray> ray;
    Field* fld0 = opt_sys_->optical_spec()->field_of_view()->field(0);

    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    // collect l_prime for on-axial data
    std::vector<double> l_primes;
    for(int wi = 0; wi < num_wvls; wi++){
        std::shared_ptr<ParaxialRay> ax_ray = opt_sys_->axial_ray(wi);
        double l_prime = ax_ray->back()->l_prime();
        l_primes.push_back(l_prime);
    }


    // collect zonal data
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> py;
        std::vector<double> lsa;

        py.push_back(0.0);
        lsa.push_back(l_primes[wi]);

        std::vector<double> y_list;
        std::vector<double> N_list;
        std::vector<double> M_list;

        for(int ri = 1; ri < num_data; ri++) {
            pupil(0) = 0.0;
            pupil(1) = (double)ri/(double)(num_data-1);
            ray = tracer->trace_pupil_ray(pupil, fld0, wvl);

            py.push_back(pupil(1));

            double y = ray->back()->y();
            double M = ray->back()->M();
            double N = ray->back()->N();
            lsa.push_back(-y*N/M);

            y_list.push_back(y);
            M_list.push_back(M);
            N_list.push_back(N);

        }

        // interpolation
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

void Aberration::plot_astigmatism(int ray_aiming_type, double scale)
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


    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    Field *tmp_fld = new Field();

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvls; wi++){

        // collect data (10 points)
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> fy;
        std::vector<double> xfo;
        std::vector<double> yfo;

        for(int fi = 0; fi < num_data; fi++) {
            double y = maxfld*(double)fi/(double)(num_data-1);
            tmp_fld->set_y(y);

            PupilCrd aim_pt = Eigen::Vector2d::Zero(2);

            if(ray_aiming_type == 0){ // stop
                aim_pt = tracer->aim_chief_ray(tmp_fld, wvl);
            }else{ // center of upper/lower ray
                double vuy = tracer->compute_vignetting_factor_for_pupil(PupilCrd({0.0, 1.0}), *tmp_fld);
                double vly = tracer->compute_vignetting_factor_for_pupil(PupilCrd({0.0, -1.0}), *tmp_fld);

                aim_pt = {0.0, (vuy-vly)/2.0};
            }

            tmp_fld->set_aim_pt(aim_pt);
            fy.push_back(y);

            Eigen::Vector2d s_t = tracer->trace_coddington(tmp_fld, wvl);

            xfo.push_back(s_t(0));
            yfo.push_back(s_t(1));
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

    std::shared_ptr<ParaxialRay> ref_prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, ref_wvl);
    double y = ref_prx_ray->at(last_surf)->y();
    double u_prime = ref_prx_ray->at(img_srf)->u_prime();
    double ref_l_prime = -y/u_prime;


    std::vector<double> xdata, ydata;

    for(int i = 0; i < num_data; i++) {
        double wvl = lower_wvl + i*wvl_step;
        std::shared_ptr<ParaxialRay> prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);

        y = prx_ray->at(last_surf)->y();
        u_prime = prx_ray->at(img_srf)->u_prime();
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


void Aberration::plot_spot_diagram(int pattern, int nrd, double scale, double dot_size)
{
    renderer_->set_grid_layout(num_fld_,1);

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    std::vector<Eigen::Vector2d> pupils;

    switch (pattern) {
    case Aberration::SpotRayPattern::Grid :
        pupils = create_grid_circle(nrd);
        break;
    case Aberration::SpotRayPattern::Hexapolar :
        pupils = create_hexapolar(nrd);
        break;
    default:
        throw "Undefined spot pattern";
    }

    std::vector<double> ray_dx, ray_dy;
    std::vector< std::shared_ptr<Ray> > rays;

    const WvlSpec* wvl_spec = opt_sys_->optical_spec()->spectral_region();

    for(int fi = 0; fi < num_fld_; fi++){
        renderer_->set_current_cell(num_fld_ - fi -1, 0);

        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);

        // trace chief ray
        //pupil << 0.0, 0.0;
        //Ray chief_ray = tracer->trace_pupil_ray(pupil, fi, ref_wvl_idx_);
        std::shared_ptr<Ray> chief_ray = opt_sys_->reference_ray(1, fi, ref_wvl_idx_);
        double chief_ray_x = chief_ray->back()->x();
        double chief_ray_y = chief_ray->back()->y();

        for(int wi = 0; wi < num_wvl_; wi++){
            double wvl = wvl_spec->wvl(wi)->value();

            int valid_ray_count = tracer->trace_pupil_pattern_rays(rays, pupils, fld, wvl);

            ray_dx.clear();
            ray_dy.clear();

            ray_dx.reserve(valid_ray_count);
            ray_dy.reserve(valid_ray_count);

            for(auto& ray : rays){
                if(ray->status() == RayStatus::PassThrough){
                    ray_dx.push_back(ray->back()->x() - chief_ray_x);
                    ray_dy.push_back(ray->back()->y() - chief_ray_y);
                }
            }

            Rgb color = wvl_spec->wvl(wi)->render_color();
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



std::vector<Eigen::Vector2d> Aberration::create_grid_circle(int nrd)
{
    std::vector<Eigen::Vector2d> pupils;
    Eigen::Vector2d pupil;

    for(int pi = 0; pi < nrd; pi++) {
        pupil(0) = -1.0 + (double)pi*2.0/(double)(nrd-1);

        for(int pj = 0; pj < nrd; pj++) {
            pupil(1) = -1.0 + (double)pj*2.0/(double)(nrd-1);

            //double r = sqrt( pow(pupil(0),2) + pow(pupil(1),2) );
            double r2 = pow(pupil(0),2) + pow(pupil(1),2);
            if(r2 <= 1.0){
                pupils.push_back(pupil);
            }
        }
    }

    return pupils;
}

std::vector<Eigen::Vector2d> Aberration::create_hexapolar(int nrd)
{
    std::vector<Eigen::Vector2d> pupils;
    Eigen::Vector2d pupil;

    int half_num_rings = nrd/2;
    for (int r = 0; r < nrd/2; r++){
        int num_rays_in_ring = 6*r;
        if(num_rays_in_ring == 0){
            pupil(0) = 0.0;
            pupil(1) = 0.0;
            pupils.push_back(pupil);
            continue;
        }

        double ang_step = 2*M_PI/(double)num_rays_in_ring;

        for(int ai = 0; ai < num_rays_in_ring; ai++){
            pupil(0) = (double)r * 1.0/(half_num_rings) * cos((double)ai*ang_step);
            pupil(1) = (double)r * 1.0/(half_num_rings) * sin((double)ai*ang_step);
            pupils.push_back(pupil);
        }
    }

    return pupils;
}
