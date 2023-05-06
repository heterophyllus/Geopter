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
    const int stop_index = opt_sys_->GetOpticalAssembly()->StopIndex();
    //const double stop_radius = opt_sys_->optical_assembly()->surface(stop_index)->max_aperture();

    Eigen::Vector2d pupil;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->SetApertureCheck(false);
    tracer->SetApplyVig(true);

    auto plot_data = std::make_shared<PlotData>();
    plot_data->SetTitle("Transverse Aberration");

    if(pupil_dir == 0){
        plot_data->SetXLabel("x at stop");
    }else{
        plot_data->SetXLabel("y at stop");
    }

    if(abr_dir == 0){
        plot_data->SetYLabel("dx");
    }else{
        plot_data->SetYLabel("dy");
    }

    SequentialPath ref_seq_path = tracer->CreateSequentialPath(ref_wvl_val_);

    // trace chief ray
    auto chief_ray = std::make_shared<Ray>();
    chief_ray->Allocate(ref_seq_path.Size());

    int trace_result = tracer->TracePupilRay(chief_ray, ref_seq_path, Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val_);
    if(TRACE_SUCCESS != trace_result){
        std::cerr << "Failed to trace chief ray: " << std::endl;
        delete tracer;
        return plot_data;
    }

    double x0 = chief_ray->GetBack()->X();
    double y0 = chief_ray->GetBack()->Y();

    // trace zonal rays for all wavelengths
    for(int wi = 0; wi < num_wvl_; wi++)
    {
        double wvl = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();
        Rgb render_color = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->RenderColor();

        SequentialPath seq_path = tracer->CreateSequentialPath(wvl);

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

            auto ray = std::make_shared<Ray>();
            ray->Allocate(seq_path.Size());

            if(TRACE_SUCCESS != tracer->TracePupilRay(ray, seq_path, pupil, fld, wvl)){
                continue;
            }

            if(ray->Status() == TRACE_SUCCESS){

                if(pupil_dir == 0) {
                    double x_at_stop = ray->GetSegmentAt(stop_index)->X();
                    pupil_data.push_back(x_at_stop);
                }else{
                    double y_at_stop = ray->GetSegmentAt(stop_index)->Y();
                    pupil_data.push_back(y_at_stop);
                }

                if(abr_dir == 0) { // dx
                    double x = ray->GetBack()->X();
                    abr_data.push_back(x - x0);

                }else{ // dy
                    double y = ray->GetBack()->Y();
                    abr_data.push_back(y - y0);
                }

            }

        }

        auto graph = std::make_shared<Graph2d>(pupil_data, abr_data);
        graph->SetRenderColor(render_color);
        graph->SetName( "W" + std::to_string(wi) );
        plot_data->AddGraph(graph);
    }

    delete tracer;

    return plot_data;
}
