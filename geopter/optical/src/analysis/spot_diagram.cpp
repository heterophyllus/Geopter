#include <iostream>
#include "analysis/spot_diagram.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"
#include "renderer/renderer.h"

using namespace geopter;

SpotDiagram::SpotDiagram(OpticalSystem* opt_sys):
    RayAberration(opt_sys)
{

}

std::shared_ptr<PlotData> SpotDiagram::plot(const Field* fld, int pattern, int nrd, double dot_size)
{
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    std::vector<Eigen::Vector2d> pupils;

    switch (pattern) {
    case SpotDiagram::SpotRayPattern::Grid :
        pupils = create_grid_circle(nrd);
        break;
    case SpotDiagram::SpotRayPattern::Hexapolar :
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

        auto graph = std::make_shared<Graph2d>(ray_dx, ray_dy, color, Renderer::LineStyle::Dots, dot_size);
        graph->set_name(std::to_string(wvl) + "nm");

        plot_data->add_graph(graph);
    }


    delete tracer;

    return plot_data;
}



std::vector<Eigen::Vector2d> SpotDiagram::create_grid_circle(int nrd)
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

std::vector<Eigen::Vector2d> SpotDiagram::create_hexapolar(int nrd)
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
