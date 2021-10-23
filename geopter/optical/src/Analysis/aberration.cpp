#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>

#include "spline.h"

#include "Analysis/aberration.h"

#include "Renderer/rgb.h"

#include "System/optical_system.h"

#include "Sequential/sequential_trace.h"
#include "Sequential/trace_error.h"

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

std::shared_ptr<PlotData> Aberration::plot_transverse_aberration(double nrd, const Field* fld, int pupil_dir, int abr_dir)
{
    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    //const double stop_radius = opt_sys_->optical_assembly()->surface(stop_index)->max_aperture();

    Eigen::Vector2d pupil;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(false);
    tracer->set_apply_vig(true);

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Transverse Aberration");

    if(pupil_dir == 0){
        plot_data->set_x_axis_label("x at stop");
    }else{
        plot_data->set_x_axis_label("y at stop");
    }

    if(abr_dir == 0){
        plot_data->set_y_axis_label("dx");
    }else{
        plot_data->set_y_axis_label("dy");
    }

    // trace chief ray
    std::shared_ptr<Ray> chief_ray;
    try{
        chief_ray = tracer->trace_pupil_ray(Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val_);
    }catch(TraceError &e){
        std::cerr << "Failed to trace chief ray: " << e.cause_str() << std::endl;
        delete tracer;
        return plot_data;
    }

    double x0 = chief_ray->back()->x();
    double y0 = chief_ray->back()->y();

    // trace zonal rays for all wavelengths
    for(int wi = 0; wi < num_wvl_; wi++)
    {
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb render_color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> pupil_data;
        std::vector<double> abr_data;

        for(int ri = 0; ri < nrd; ri++)
        {
            if(pupil_dir == 0){ // sagittal
                pupil(0) = -1.0 + (double)ri*2.0/(double)(nrd-1);
                pupil(1) = 0.0;
            }else{  // tangential
                pupil(0) = 0.0;
                pupil(1) = -1.0 + (double)ri*2.0/(double)(nrd-1);
            }

            std::shared_ptr<Ray> ray;
            try{
                ray = tracer->trace_pupil_ray(pupil, fld, wvl);
            }catch(TraceError &e){
                ray = e.ray();
                break;
            }

            if(ray->status() == RayStatus::PassThrough){

                if(pupil_dir == 0) {
                    double x_at_stop = ray->at(stop_index)->x();
                    pupil_data.push_back(x_at_stop);
                }else{
                    double y_at_stop = ray->at(stop_index)->y();
                    pupil_data.push_back(y_at_stop);
                }

                if(abr_dir == 0) { // dx
                    double x = ray->back()->x();
                    abr_data.push_back(x - x0);

                }else{ // dy
                    double y = ray->back()->y();
                    abr_data.push_back(y - y0);
                }

            }

        }

        auto point_set = std::make_shared<PointSet>(pupil_data, abr_data, render_color);
        point_set->set_name( std::to_string(wvl) + "nm" );
        plot_data->add_pointset(point_set);
    }

    delete tracer;

    return plot_data;
}

std::shared_ptr<PlotData> Aberration::plot_longitudinal_spherical_aberration(int num_rays)
{
    const Field* fld0 = opt_sys_->optical_spec()->field_of_view()->field(0);
    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    // collect l_prime for on-axial data
    std::vector<double> l_primes;
    for(int wi = 0; wi < num_wvls; wi++){
        std::shared_ptr<ParaxialRay> ax_ray = opt_sys_->axial_ray(wi);
        double l_prime = ax_ray->back()->l_prime();
        l_primes.push_back(l_prime);
    }


    // collect zonal data
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    Eigen::Vector2d pupil;

    auto plotdata = std::make_shared<PlotData>();
    plotdata->set_title("Longitudinal Spherical Aberration");
    plotdata->set_x_axis_label("Spherical Aberration");
    plotdata->set_y_axis_label("Pupil Height");

    for(int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> py;
        std::vector<double> lsa;

        // on axis
        py.push_back(0.0);
        lsa.push_back(l_primes[wi]);

        for(int ri = 1; ri < num_rays; ri++) {
            pupil(0) = 0.0;
            pupil(1) = (double)ri/(double)(num_rays-1);

            std::shared_ptr<Ray> ray;
            try{
                ray = tracer->trace_pupil_ray(pupil, fld0, wvl);
            }catch(TraceError &e){
                std::cerr << "Failed to trace ray: " << "pupil= (" << pupil(0) << "," << pupil(1) << ")" << std::endl;
                std::cerr << e.cause_str() << std::endl;
                continue;
            }

            py.push_back(pupil(1));

            double y = ray->back()->y();
            double M = ray->back()->M();
            double N = ray->back()->N();
            lsa.push_back(-y*N/M);
        }

        /*
        // interpolation
        std::vector<double> spy(30);
        std::vector<double> slsa(30);

        tk::spline spline_lsa;
        spline_lsa.set_points(py, lsa);

        for(int i = 0; i < 30; i++) {
            spy[i] = (double)i/(30.0 - 1.0);
            slsa[i] = spline_lsa(spy[i]);
        }

        auto pointset = std::make_shared<PointSet>(slsa, spy, color);
        */

        auto pointset = std::make_shared<PointSet>(lsa, py, color);
        pointset->set_name(std::to_string(wvl) + "nm");

        plotdata->add_pointset(pointset);        
    }

    delete tracer;

    return plotdata;
}

std::shared_ptr<PlotData> Aberration::plot_astigmatism(int ray_aiming_type, int num_rays)
{
    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    const double h_stop = opt_sys_->optical_assembly()->surface(stop_index)->semi_diameter();

    double maxfld = opt_sys_->optical_spec()->field_of_view()->max_field();

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Astigmatism");
    plot_data->set_x_axis_label("astigmatism");
    plot_data->set_y_axis_label("field");

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_apply_vig(true);
    tracer->set_aperture_check(true);

    // create temporary fields
    Field *tmp_fld;
    std::vector<Field*> tmp_flds;
    std::vector<double> pupil_offsets;
    std::vector<double> ys;

    tmp_flds.reserve(num_rays);
    pupil_offsets.reserve(num_rays);
    ys.reserve(num_rays);

    if(ray_aiming_type == 0){ // chief ray
        for(int fi = 0; fi < num_rays; fi++){
            tmp_fld = new Field;

            double y = maxfld*(double)fi/(double)(num_rays-1);
            ys.push_back(y);
            tmp_fld->set_y(y);

            tmp_flds.push_back(tmp_fld);
            pupil_offsets.push_back(0.0);
        }
    }else{ // intermidiate ray
        for(int fi = 0; fi < num_rays; fi++){
            tmp_fld = new Field;

            double y = maxfld*(double)fi/(double)(num_rays-1);
            ys.push_back(y);
            tmp_fld->set_y(y);

            try{
                Eigen::Vector2d aim_pt = tracer->aim_chief_ray(tmp_fld, ref_wvl_val_);
                tmp_fld->set_aim_pt(aim_pt);
            }catch(TraceRayAimingFailedError &e){
                std::cerr << e.cause_str() << std::endl;
                continue;
            }catch(...){
                std::cerr << "Unknown error" << std::endl;
                continue;
            }

            double vuy = tracer->compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, 1.0}), *tmp_fld);
            double vly = tracer->compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, -1.0}), *tmp_fld);
            tmp_fld->set_vuy(vuy);
            tmp_fld->set_vly(vly);

            tmp_flds.push_back(tmp_fld);

            auto ray_upper = tracer->trace_pupil_ray(Eigen::Vector2d({0.0, 1.0}), tmp_fld, ref_wvl_val_);
            auto ray_lower = tracer->trace_pupil_ray(Eigen::Vector2d({0.0, -1.0}), tmp_fld, ref_wvl_val_);

            double h_upper = ray_upper->at(stop_index)->y();
            double h_lower = ray_lower->at(stop_index)->y();

            double pupil_offset = (h_upper + h_lower)/2.0/h_stop;
            pupil_offsets.push_back(pupil_offset);
        }
    }


    // trace coddington for all wavelengths
    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    for(int wi = 0; wi < num_wvls; wi++){

        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> fy;
        std::vector<double> xfo;
        std::vector<double> yfo;

        for(int fi = 0; fi < num_rays; fi++) {
            tmp_fld = tmp_flds[fi];
            double pupil_offset = pupil_offsets[fi];

            fy.push_back(ys[fi]);

            try{
                Eigen::Vector2d s_t = tracer->trace_coddington(tmp_fld, wvl, pupil_offset);

                xfo.push_back(s_t(0));
                yfo.push_back(s_t(1));
            }catch(TraceError &e){
                std::cerr << "Failed to trace coddington: " << "Field " << fi << ": " << e.cause_str() << std::endl;
                continue;
            }
        }

        auto point_set_sag = std::make_shared<PointSet>(xfo, fy, color,Renderer::LineStyle::Solid);
        point_set_sag->set_name(std::to_string(wvl) + "nm - S");

        auto point_set_tan = std::make_shared<PointSet>(yfo, fy, color,Renderer::LineStyle::Dots);
        point_set_tan->set_name(std::to_string(wvl) + "nm - T");

        plot_data->add_pointset(point_set_sag);
        plot_data->add_pointset(point_set_tan);
    }

    for(auto& fld : tmp_flds){
        delete fld;
    }

    pupil_offsets.clear();
    ys.clear();

    delete tracer;

    return plot_data;
}

std::shared_ptr<PlotData> Aberration::plot_chromatic_focus_shift(double lower_wvl, double higher_wvl)
{
    constexpr int num_data = 100;
    double wvl_step = (higher_wvl-lower_wvl)/(double)(num_data-1);

    double u0, y0, ubar0, ybar0;

    ParaxialTrace *tracer = new ParaxialTrace(opt_sys_);
    tracer->get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    auto ref_prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, ref_wvl_val_);
    double ref_l_prime = ref_prx_ray->back()->l_prime();

    std::vector<double> xdata, ydata;
    xdata.reserve(num_data);
    ydata.reserve(num_data);

    for(int i = 0; i < num_data; i++) {
        double wvl = lower_wvl + (double)i*wvl_step;
        auto prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        double l_prime = prx_ray->back()->l_prime();

        xdata.push_back(wvl);
        ydata.push_back(l_prime - ref_l_prime);
    }

    delete tracer;

    auto point_set = std::make_shared<PointSet>(xdata, ydata, rgb_black, Renderer::LineStyle::Solid);
    point_set->set_name("Chromatic focus shift");

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Chromatic focus shift");
    plot_data->set_x_axis_label("Wavelength(nm)");
    plot_data->set_y_axis_label("Focus Shift");
    plot_data->add_pointset(point_set);

    return plot_data;
}


std::shared_ptr<PlotData> Aberration::plot_spot_diagram(const Field* fld, int pattern, int nrd, double dot_size)
{
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


    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Spot");
    plot_data->set_x_axis_label("dx");
    plot_data->set_y_axis_label("dy");


    // trace chief ray
    std::shared_ptr<Ray> chief_ray;
    try{
        chief_ray = tracer->trace_pupil_ray(Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val_);
    }catch(TraceError &e){
        std::cerr << "Failed to trace chief ray" << std::endl;
        delete tracer;
        return plot_data;
    }

    //auto chief_ray = opt_sys_->reference_ray(1, fi, ref_wvl_idx_);
    double chief_ray_x = chief_ray->back()->x();
    double chief_ray_y = chief_ray->back()->y();

    // trace patterned rays for all wavelengths
    std::vector<double> ray_dx, ray_dy;
    std::vector< std::shared_ptr<Ray> > rays;

    for(int wi = 0; wi < num_wvl_; wi++){

        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

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

        // TODO: calculate RMS, Max diameter, etc

        auto point_set = std::make_shared<PointSet>(ray_dx, ray_dy, color, Renderer::LineStyle::Dots, dot_size);
        point_set->set_name(std::to_string(wvl) + "nm");

        plot_data->add_pointset(point_set);
    }


    delete tracer;

    return plot_data;
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
