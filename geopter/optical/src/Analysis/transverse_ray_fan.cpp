#include <iostream>
#include "analysis/transverse_ray_fan.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"

using namespace geopter;

TransverseRayFan::TransverseRayFan(OpticalSystem* opt_sys) :
    RayAberration(opt_sys)
{

}


std::shared_ptr<PlotData> TransverseRayFan::plot(double nrd, const Field* fld, int pupil_dir, int abr_dir)
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
