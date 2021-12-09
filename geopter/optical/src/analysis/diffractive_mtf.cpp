#include <numeric>
#include "analysis/diffractive_mtf.h"
#include "analysis/diffractive_psf.h"
#include "common/circ_shift.h"
#include "common/matrix_tool.h"
#include "renderer/renderer.h"

using namespace geopter;

DiffractiveMTF::DiffractiveMTF(OpticalSystem *opt_sys) :
    WaveAberration(opt_sys)
{

}

std::shared_ptr<PlotData> DiffractiveMTF::plot(OpticalSystem* opt_sys, int M, double L)
{
    const int num_flds = opt_sys->optical_spec()->field_of_view()->field_count();
    const int num_wvls = opt_sys->optical_spec()->spectral_region()->wvl_count();
    std::vector<double> wvl_list = opt_sys->optical_spec()->spectral_region()->get_wavelength_list();
    std::vector<double> wt_list = opt_sys->optical_spec()->spectral_region()->get_weight_list();

    const double sum_wt = std::accumulate(wt_list.begin(), wt_list.end(), 0);

    const double du = L/static_cast<double>(M);

    std::vector<double> fu;

    for(int i = 0; i < M; i++){
        //fu.push_back( -1.0/(2.0*du) + static_cast<double>(i)*1.0/L );
        fu.push_back( 0.0 + static_cast<double>(i)*1.0/L );
    }

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_plot_style(Renderer::PlotStyle::Curve);
    plot_data->set_title("FFT MTF");
    plot_data->set_x_axis_label("Spatial Frequency");
    plot_data->set_y_axis_label("MTF");

    DiffractivePSF *psf_analyzer = new DiffractivePSF(opt_sys);


    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->optical_spec()->field_of_view()->field(fi);
        Eigen::MatrixXd psf = Eigen::MatrixXd::Zero(M,M);

        for(int wi = 0; wi < num_wvls; wi++) {
            double wvl = wvl_list[wi];
            double wt = wt_list[wi];
            psf_analyzer->from_opd_trace(opt_sys, fld, wvl, M, L);
            Eigen::MatrixXd psf_for_wvl = psf_analyzer->to_matrix();
            psf += wt*psf_for_wvl;
        }

        psf = psf/sum_wt;

        Eigen::MatrixXd psf2 = psf.array().pow(2);
        Eigen::MatrixXcd psf2_c = psf2.cast<std::complex<double>>();

        Eigen::MatrixXcd temp1 = fftshift(psf2_c);
        Eigen::MatrixXcd temp2 = MatrixTool::fft2(temp1);
        Eigen::MatrixXd temp3 = temp2.array().abs();
        double mtf0 = temp3(0,0);
        Eigen::MatrixXd mtf = temp3.array()/mtf0;

        std::vector<double> mtf_tan = MatrixTool::to_std_vector(mtf.col(0));
        std::vector<double> mtf_sag = MatrixTool::to_std_vector(mtf.row(0));

        auto graph_sag = std::make_shared<Graph2d>();
        graph_sag->set_data(fu,mtf_sag);
        graph_sag->set_line_style(Renderer::LineStyle::Solid);
        graph_sag->set_render_color(fld->render_color());
        graph_sag->set_name("MTF_S F" + std::to_string(fi));

        auto graph_tan = std::make_shared<Graph2d>();
        graph_tan->set_data(fu,mtf_tan);
        graph_tan->set_line_style(Renderer::LineStyle::Dots);
        graph_tan->set_render_color(fld->render_color());
        graph_tan->set_name("MTF_T F" + std::to_string(fi));

        plot_data->add_graph(graph_sag);
        plot_data->add_graph(graph_tan);

    }

    delete psf_analyzer;


    return plot_data;
}

