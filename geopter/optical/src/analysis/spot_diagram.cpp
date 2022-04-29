#define _USE_MATH_DEFINES
#include <cmath>
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

std::shared_ptr<PlotData> SpotDiagram::plot(const Field* fld, int pattern, int max_nrd, double dot_size)
{
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Spot");
    plot_data->set_x_axis_label("dx");
    plot_data->set_y_axis_label("dy");
    plot_data->set_plot_style(Renderer::PlotStyle::Scatter);

    // weight list
    const int num_wvls = opt_sys_->fundamental_data().number_of_wavelengths;
    std::vector<double> wvl_weights(num_wvls);
    for(int wi = 0; wi < num_wvls; wi++){
        wvl_weights[wi] = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->weight();
    }

    double max_wt = *std::max_element(wvl_weights.begin(), wvl_weights.end());

    SequentialPath ref_seq_path = tracer->sequential_path(ref_wvl_val_);
    // trace chief ray
    auto chief_ray = std::make_shared<Ray>(ref_seq_path.size());
    if(TRACE_SUCCESS != tracer->trace_pupil_ray(chief_ray, ref_seq_path, Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val_) ){
        std::cerr << "Failed to trace chief ray" << std::endl;
        delete tracer;
        return plot_data;
    }

    double chief_ray_x = chief_ray->back()->x();
    double chief_ray_y = chief_ray->back()->y();


    // trace patterned rays for all wavelengths
    Eigen::Vector2d pupil;

    auto ray = std::make_shared<Ray>(chief_ray->size());

    for(int wi = 0; wi < num_wvl_; wi++){

        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        auto graph = std::make_shared<Graph2d>();
        graph->set_name(std::to_string(wvl) + "nm");
        graph->set_line_style(Renderer::LineStyle::Dots);
        graph->set_render_color(color);
        graph->set_line_width(dot_size);

        SequentialPath seq_path = tracer->sequential_path(wvl);

        // calculate nrd for current wvl
        int nrd = (wvl_weights[wi]/max_wt)*max_nrd;

        if(SpotDiagram::SpotRayPattern::Grid == pattern)
        {            
            graph->resize(nrd*nrd);

            int valid_ray_count = 0;

            for(int i = 0; i < nrd; i++){
                for(int j = 0; j < nrd; j++){
                    pupil(0) = -1.0 + static_cast<double>(j)*2.0/static_cast<double>(nrd-1);
                    pupil(1) = -1.0 + static_cast<double>(i)*2.0/static_cast<double>(nrd-1);

                    if(pupil.norm() <= 1.0){

                        ray->set_status(RayStatus::PassThrough);
                        tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);

                        if(RayStatus::PassThrough == ray->status()){

                            double ray_x = ray->back()->x();
                            double ray_y = ray->back()->y();

                            double dx = ray_x - chief_ray_x;
                            double dy = ray_y - chief_ray_y;

                            graph->set_data(valid_ray_count, dx, dy);

                            valid_ray_count++;
                        }
                    }
                }


            }

            graph->resize(valid_ray_count);

        }else if(SpotDiagram::SpotRayPattern::Hexapolar == pattern){

            graph->resize(HexapolarArray<double>(nrd).total_number_of_points());
            int valid_ray_count = 0;

            int half_num_rings = nrd/2;
            for (int r = 0; r < nrd/2; r++)
            {
                int num_rays_in_ring = 6*r;

                // center of hexapolar
                if(num_rays_in_ring == 0){
                    pupil(0) = 0.0;
                    pupil(1) = 0.0;

                    ray->set_status(RayStatus::PassThrough);
                    tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);

                    double dx = ray->back()->x() - chief_ray_x;
                    double dy = ray->back()->y() - chief_ray_y;

                    graph->set_data(valid_ray_count, dx, dy);
                    valid_ray_count++;

                    continue;
                }

                double ang_step = 2*M_PI/(double)num_rays_in_ring;

                for(int ai = 0; ai < num_rays_in_ring; ai++){
                    pupil(0) = (double)r * 1.0/(half_num_rings) * cos((double)ai*ang_step);
                    pupil(1) = (double)r * 1.0/(half_num_rings) * sin((double)ai*ang_step);

                    ray->set_status(RayStatus::PassThrough);
                    tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);

                    if(RayStatus::PassThrough == ray->status()){
                        double dx = ray->back()->x() - chief_ray_x;
                        double dy = ray->back()->y() - chief_ray_y;

                        graph->set_data(valid_ray_count, dx, dy);
                        valid_ray_count++;
                    }
                }
            }

            graph->resize(valid_ray_count);

        }else{
            std::cerr << "Undefined spot pattern" << std::endl;
        }

        // TODO: calculate RMS, Max diameter, etc


        plot_data->add_graph(graph);
    }

    delete tracer;

    return plot_data;
}

