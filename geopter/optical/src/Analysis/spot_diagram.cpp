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

    double chief_ray_x = chief_ray->back()->x();
    double chief_ray_y = chief_ray->back()->y();

    // trace patterned rays for all wavelengths
    int num_data = nrd*nrd;
    switch (pattern) {
    case SpotDiagram::SpotRayPattern::Grid:
        num_data = nrd*nrd;
        break;
    case SpotDiagram::SpotRayPattern::Hexapolar:
        num_data = HexapolarArray<double>(nrd).total_number_of_points();
        break;
    }

    Eigen::Vector2d pupil;

    auto ray = std::make_shared<Ray>(chief_ray->size());

    for(int wi = 0; wi < num_wvl_; wi++){

        std::vector<double> ray_dx; ray_dx.reserve(num_data);
        std::vector<double> ray_dy; ray_dy.reserve(num_data);

        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        SequentialPath seq_path = tracer->sequential_path(wvl);

        if(SpotDiagram::SpotRayPattern::Grid == pattern)
        {            
            for(int i = 0; i < nrd; i++){
                for(int j = 0; j < nrd; j++){
                    pupil(0) = -1.0 + (double)j*2.0/(double)(nrd-1);
                    pupil(1) = -1.0 + (double)i*2.0/(double)(nrd-1);

                    ray->set_status(RayStatus::PassThrough);

                    double r2 = pow(pupil(0),2) + pow(pupil(1),2);
                    if(r2 < 1.0){
                        try{
                            tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);
                            ray->set_wvl(wvl);
                            ray->set_pupil_coord(pupil);
                        }catch(TraceError &e){
                            continue;
                        }

                        if(RayStatus::PassThrough == ray->status()){
                            double ray_x = ray->back()->x();
                            double ray_y = ray->back()->y();

                            ray_dx.push_back(ray_x-chief_ray_x);
                            ray_dy.push_back(ray_y-chief_ray_y);
                        }
                    }
                }
            }


        }else if(SpotDiagram::SpotRayPattern::Hexapolar == pattern){
            int half_num_rings = nrd/2;
            for (int r = 0; r < nrd/2; r++)
            {
                int num_rays_in_ring = 6*r;

                // center of hexapolar
                if(num_rays_in_ring == 0){
                    pupil(0) = 0.0;
                    pupil(1) = 0.0;
                    try{
                        tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);
                        ray->set_wvl(wvl);
                        ray->set_pupil_coord(pupil);
                    }catch(TraceError &e){

                    }

                    ray_dx.push_back( ray->back()->x() - chief_ray_x );
                    ray_dy.push_back( ray->back()->y() - chief_ray_y );

                    continue;
                }

                double ang_step = 2*M_PI/(double)num_rays_in_ring;

                for(int ai = 0; ai < num_rays_in_ring; ai++){
                    pupil(0) = (double)r * 1.0/(half_num_rings) * cos((double)ai*ang_step);
                    pupil(1) = (double)r * 1.0/(half_num_rings) * sin((double)ai*ang_step);
                    ray->set_status(RayStatus::PassThrough);
                    try{
                        tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);
                        ray->set_wvl(wvl);
                        ray->set_pupil_coord(pupil);
                    }catch(TraceError &e){
                        continue;
                    }

                    if(RayStatus::PassThrough == ray->status()){
                        ray_dx.push_back( ray->back()->x() - chief_ray_x );
                        ray_dy.push_back( ray->back()->y() - chief_ray_y );
                    }
                }
            }


        }else{
            std::cerr << "Undefined spot pattern" << std::endl;
        }

        // TODO: calculate RMS, Max diameter, etc

        auto graph = std::make_shared<Graph2d>(ray_dx, ray_dy, color, Renderer::LineStyle::Dots, dot_size);
        graph->set_name(std::to_string(wvl) + "nm");

        plot_data->add_graph(graph);
    }

    delete tracer;

    return plot_data;
}

