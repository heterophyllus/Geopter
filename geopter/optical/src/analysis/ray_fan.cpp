#include <vector>
#include <sstream>
#include <iomanip>

#include "spline.h"

#include "Analysis/ray_fan.h"

#include "Renderer/rgb.h"
#include "System/optical_system.h"
#include "Spec/optical_spec.h"
#include "Sequential/ray.h"
#include "Sequential/sequential_trace.h"


using namespace geopter;

RayFan::RayFan(OpticalSystem* opt_sys, Renderer* renderer) :
    opt_sys_(opt_sys),
    renderer_(renderer),
    total_num_data_(100)
{

}

RayFan::~RayFan()
{
    opt_sys_ = nullptr;
    renderer_ = nullptr;
}

void RayFan::plot(double scale, int nrd)
{
    const int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    renderer_->set_grid_layout(num_flds, 2);

    const double step = 2.0/(double)(nrd-1);

    PupilCrd pupil;
    PupilCrd vig_pupil;

    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();


    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    for(int fi = 0; fi < num_flds; fi++)
    {
        // trace chief ray
        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);

        std::shared_ptr<Ray> chief_ray = opt_sys_->reference_ray(1,fi,ref_wvl_idx);
        double x0 = chief_ray->back()->x();
        double y0 = chief_ray->back()->y();

        // trace zonal rays for all wavelengths
        for(int wi = 0; wi < num_wvls; wi++)
        {
            double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
            Rgb render_color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

            std::vector<double> px, py;
            std::vector<double> dx, dy;
            px.reserve(nrd);
            py.reserve(nrd);
            dx.reserve(nrd);
            dy.reserve(nrd);

            std::shared_ptr<Ray> tangential_ray;
            std::shared_ptr<Ray> sagittal_ray;

            for(int ri = 0; ri < nrd; ri++)
            {
                // tangential
                pupil(0) = 0.0;
                pupil(1) = -1.0 + (double)ri*step;

                tangential_ray = tracer->trace_pupil_ray(pupil, fld, wvl);

                if(tangential_ray->status() == RayStatus::PassThrough){
                    double y = tangential_ray->back()->y();
                    vig_pupil = fld->apply_vignetting(pupil);
                    py.push_back(vig_pupil(1));
                    dy.push_back(y - y0);
                }

                // sagittal
                pupil(0) = -1.0 + (double)ri*step;
                pupil(1) = 0.0;

                sagittal_ray = tracer->trace_pupil_ray(pupil, fld, wvl);
                if(sagittal_ray->status() == RayStatus::PassThrough){
                    double x = sagittal_ray->back()->x();
                    vig_pupil = fld->apply_vignetting(pupil);
                    px.push_back(vig_pupil(0));
                    dx.push_back(x - x0);
                }
            }

            // interpolation
            tk::spline spline_tangential;
            spline_tangential.set_points(py,dy);

            tk::spline spline_sagittal;
            spline_sagittal.set_points(px,dx);

            std::vector<double> spx(total_num_data_), spy(total_num_data_);
            std::vector<double> sdx(total_num_data_), sdy(total_num_data_);
            double px_min = - (1.0 - fld->vlx());
            double px_max = 1.0 - fld->vux();
            double py_min = -(1.0 - fld->vly());
            double py_max = 1.0 - fld->vuy();
            for(int k = 0; k < total_num_data_; k++){
                spx[k] = px_min + (double)k*(px_max-px_min)/(double)(total_num_data_ -1);
                spy[k] = py_min + (double)k*(py_max-py_min)/(double)(total_num_data_ -1);
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

