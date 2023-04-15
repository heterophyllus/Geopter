#define _USE_MATH_DEFINES
#include <cmath>

#include "analysis/geometrical_mtf.h"
#include "sequential/sequential_trace.h"
#include "renderer/renderer.h"


namespace  {


double CalculateGeometricalMtf(double s, double t, const std::vector<double>& x, const std::vector<double>& y)
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

    const int num_flds = opt_sys->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    const int num_wvls = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();

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
    plot_data->SetPlotStyle(0);
    plot_data->SetTitle("Geometrical MTF");
    plot_data->SetXLabel("Frequency");
    plot_data->SetYLabel("MTF");

    SequentialTrace *tracer = new SequentialTrace(opt_sys);
    tracer->SetApertureCheck(true);
    tracer->SetApplyVig(false);

    std::vector<SequentialPath> seq_paths;
    for (int wi = 0; wi < num_wvls; wi++){
        double wvl = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();
        seq_paths.emplace_back(tracer->CreateSequentialPath(wvl));
    }

    const int ref_wvl_idx = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->ReferenceIndex();
    const double ref_wvl_val = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    const SequentialPath ref_seq_path = seq_paths[ref_wvl_idx];


    auto chief_ray = std::make_shared<Ray>( opt_sys->GetOpticalAssembly()->NumberOfSurfaces() );
    auto ray = std::make_shared<Ray>(opt_sys->GetOpticalAssembly()->NumberOfSurfaces());

    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->GetOpticalSpec()->GetFieldSpec()->GetField(fi);

        if(TRACE_SUCCESS != tracer->TracePupilRay(chief_ray, ref_seq_path, Eigen::Vector2d({0.0,0.0}), fld, ref_wvl_val) ){
            std::cerr << "Failed to trace chief ray" << std::endl;
            continue;
        }

        double chief_ray_x = chief_ray->GetBack()->X();
        double chief_ray_y = chief_ray->GetBack()->Y();


        std::vector<double> us, vs; //point coordinates on image in current field
        us.clear();
        vs.clear();

        us.reserve(nrd*nrd);
        vs.reserve(nrd*nrd);

        Eigen::Vector2d pupil;
        const double step = 2.0/static_cast<double>(nrd-1);

        for(int wi = 0; wi < num_wvls; wi++){
            double wvl = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();

            for (int pi = 0; pi < nrd; pi++){
                for(int pj = 0; pj < nrd; pj++){
                    pupil(0) = -1.0 + static_cast<double>(pj)*step;
                    pupil(1) = -1.0 + static_cast<double>(pi)*step;

                    if(pupil.norm() <= 1.00){
                        ray->SetStatus(TRACE_SUCCESS);
                        if(TRACE_SUCCESS == tracer->TracePupilRay(ray,seq_paths[wi], pupil, fld, wvl)){
                            double ray_x = ray->GetBack()->X();
                            double ray_y = ray->GetBack()->Y();

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
            double mtf_sag = CalculateGeometricalMtf(freq, 0.0, us, vs);
            double mtf_tan = CalculateGeometricalMtf(0.0, freq, us, vs);

            mtf_sag_list[fk] = mtf_sag;
            mtf_tan_list[fk] = mtf_tan;
        }

        std::shared_ptr<Graph2d> graph_tan = std::make_shared<Graph2d>();
        graph_tan->SetData(freqs, mtf_tan_list);
        graph_tan->SetLineStyle(Renderer::LineStyle::Dots);
        graph_tan->SetRenderColor(fld->RenderColor());
        graph_tan->SetName("F" + std::to_string(fi) + "_T");

        std::shared_ptr<Graph2d> graph_sag = std::make_shared<Graph2d>();
        graph_sag->SetLineStyle(Renderer::LineStyle::Solid);
        graph_sag->SetData(freqs, mtf_sag_list);
        graph_sag->SetRenderColor(fld->RenderColor());
        graph_sag->SetName("F" + std::to_string(fi) + "_S");

        plot_data->AddGraph(graph_sag);
        plot_data->AddGraph(graph_tan);
    }

    delete tracer;

    return plot_data;
}
