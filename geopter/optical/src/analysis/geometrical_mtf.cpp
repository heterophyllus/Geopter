#define _USE_MATH_DEFINES
#include <cmath>

#include "analysis/geometrical_mtf.h"
#include "sequential/sequential_trace.h"
#include "renderer/renderer.h"


namespace  {


double calc_geo_mtf(double s, double t, const std::vector<double>& x, const std::vector<double>& y)
{
    assert(x.size() == y.size());

    const int N = x.size();

    double Rc = 0.0;
    double Rs = 0.0;
    for(int i= 0; i < N; i++){
        Rc += cos( 2.0*M_PI*(s*x[i] + t*y[i]) );
        Rs += sin( 2.0*M_PI*(s*x[i] + t*y[i]) );
    }

    Rc /= static_cast<double>(N);
    Rs /= static_cast<double>(N);

    double mtf = sqrt(Rc*Rc + Rs*Rs);

    return mtf;
}

}

using namespace geopter;

GeometricalMTF::GeometricalMTF()
{

}

std::shared_ptr<PlotData> GeometricalMTF::plot(OpticalSystem* opt_sys, int nrd, double max_freq, double freq_step)
{

    /*
     * 1. spot data
     * 2. calculate mtf for each frequency
     *
     */

    const int num_flds = FieldSpec::number_of_fields();
    const int num_wvls = WvlSpec::number_of_wavelengths();

    std::vector<double> freqs;
    {
        double freq = 0.0;
        while(freq < max_freq){
            freqs.push_back(freq);
            freq += freq_step;
        }
    }

    const int num_freqs = freqs.size();

    std::shared_ptr<PlotData> plot_data = std::make_shared<PlotData>();
    plot_data->set_plot_style(0);
    plot_data->set_title("Geometrical MTF");
    plot_data->set_x_axis_label("Frequency");
    plot_data->set_y_axis_label("MTF");

    SequentialTrace *tracer = new SequentialTrace(opt_sys);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    std::vector<SequentialPath> seq_paths;
    for (int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys->optical_spec()->spectral_region()->wvl(wi)->value();
        seq_paths.emplace_back(tracer->sequential_path(wvl));
    }

    const int ref_wvl_idx = opt_sys->optical_spec()->spectral_region()->reference_index();
    const double ref_wvl_val = opt_sys->optical_spec()->spectral_region()->reference_wvl();
    const SequentialPath ref_seq_path = seq_paths[ref_wvl_idx];


    auto chief_ray = std::make_shared<Ray>(opt_sys->optical_assembly()->number_of_surfaces());
    auto ray = std::make_shared<Ray>(opt_sys->optical_assembly()->number_of_surfaces());

    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->optical_spec()->field_of_view()->field(fi);

        if(TRACE_SUCCESS != tracer->trace_pupil_ray(chief_ray, ref_seq_path, Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val) ){
            std::cerr << "Failed to trace chief ray" << std::endl;
            continue;
        }

        double chief_ray_x = chief_ray->back()->x();
        double chief_ray_y = chief_ray->back()->y();


        std::vector<double> us, vs; //point coordinates on image in current field
        us.clear();
        vs.clear();

        us.reserve(nrd*nrd);
        vs.reserve(nrd*nrd);

        Eigen::Vector2d pupil;
        const double step = 2.0/static_cast<double>(nrd-1);

        for(int wi = 0; wi < num_wvls; wi++){
            double wvl = opt_sys->optical_spec()->spectral_region()->wvl(wi)->value();

            for (int pi = 0; pi < nrd; pi++){
                for(int pj = 0; pj < nrd; pj++){
                    pupil(0) = -1.0 + static_cast<double>(pj)*step;
                    pupil(1) = -1.0 + static_cast<double>(pi)*step;

                    if(pupil.norm() <= 1.00001){
                        ray->set_status(TRACE_SUCCESS);
                        if(TRACE_SUCCESS == tracer->trace_pupil_ray(ray,seq_paths[wi], pupil, fld, wvl)){
                            double ray_x = ray->back()->x();
                            double ray_y = ray->back()->y();

                            double dx = ray_x - chief_ray_x;
                            double dy = ray_y - chief_ray_y;

                            us.push_back(dx);
                            vs.push_back(dy);
                        }
                    }
                }


            }



        }

        std::vector<double> mtf_tan_list(num_freqs, 0.0);
        std::vector<double> mtf_sag_list(num_freqs, 0.0);

        for(int fk=0; fk < num_freqs; fk++){
            double freq = freqs[fk];
            double mtf_sag = calc_geo_mtf(freq, 0.0, us, vs);
            double mtf_tan = calc_geo_mtf(0.0, freq, us, vs);

            mtf_sag_list[fk] = mtf_sag;
            mtf_tan_list[fk] = mtf_tan;
        }

        std::shared_ptr<Graph2d> graph_tan = std::make_shared<Graph2d>();
        graph_tan->set_data(freqs, mtf_tan_list);
        graph_tan->set_line_style(Renderer::LineStyle::Dots);
        graph_tan->set_render_color(fld->render_color());
        graph_tan->set_name("F" + std::to_string(fi) + "_T");

        std::shared_ptr<Graph2d> graph_sag = std::make_shared<Graph2d>();
        graph_sag->set_line_style(Renderer::LineStyle::Solid);
        graph_sag->set_data(freqs, mtf_sag_list);
        graph_sag->set_render_color(fld->render_color());
        graph_sag->set_name("F" + std::to_string(fi) + "_S");

        plot_data->add_graph(graph_sag);
        plot_data->add_graph(graph_tan);
    }

    delete tracer;

    return plot_data;
}
