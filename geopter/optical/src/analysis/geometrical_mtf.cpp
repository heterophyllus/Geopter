#define _USE_MATH_DEFINES
#include <cmath>

#include "analysis/geometrical_mtf.h"
#include "analysis/spot_diagram.h"
#include "renderer/renderer.h"

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

    std::shared_ptr<PlotData> plot_data = std::make_shared<PlotData>();
    plot_data->set_plot_style(0);
    plot_data->set_title("Geometrical MTF");
    plot_data->set_x_axis_label("Frequency");
    plot_data->set_y_axis_label("MTF");

    SpotDiagram* spot = new SpotDiagram(opt_sys);

    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->optical_spec()->field_of_view()->field(fi);

        auto spot_data = spot->plot(fld, SpotDiagram::Grid, nrd, 1.0);

        std::vector<double> us, vs; //point coordinates on image in current field
        us.reserve(nrd*nrd*num_wvls);
        vs.reserve(nrd*nrd*num_wvls);

        for(int wi = 0; wi < num_wvls; wi++){
            us.insert(us.end(), spot_data->graph(wi)->x_data().begin(), spot_data->graph(wi)->x_data().end());
            vs.insert(vs.end(), spot_data->graph(wi)->y_data().begin(), spot_data->graph(wi)->y_data().end());
        }

        std::vector<double> mtf_tan_list, mtf_sag_list;

        for(auto freq : freqs){
            double mtf_sag = calc_geo_mtf(freq, 0.0, us, vs);
            double mtf_tan = calc_geo_mtf(0.0, freq, us, vs);

            mtf_sag_list.push_back(mtf_sag);
            mtf_tan_list.push_back(mtf_tan);
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

    delete spot;

    return plot_data;
}

double GeometricalMTF::calc_geo_mtf(double s, double t, const std::vector<double>& u, const std::vector<double>& v)
{
    assert(u.size() == v.size());

    const int N = u.size();

    double Rc = 0.0;
    double Rs = 0.0;
    for(int i= 0; i < N; i++){
        Rc += cos( 2.0*M_PI*(s*u[i] + t*v[i]) );
        Rs += sin( 2.0*M_PI*(s*u[i] + t*v[i]) );
    }

    Rc /= static_cast<double>(N);
    Rs /= static_cast<double>(N);

    double mtf = sqrt(Rc*Rc + Rs*Rs);

    return mtf;
}
