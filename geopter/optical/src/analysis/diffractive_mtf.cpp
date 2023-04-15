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

std::shared_ptr<PlotData> DiffractiveMTF::plot(OpticalSystem* opt_sys, int M)
{
    const int num_flds = opt_sys->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    const int num_wvls = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    std::vector<double> wvl_list = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->GetWavelengthList();
    std::vector<double> wt_list = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->GetWeightList();

    const double sum_wt = std::accumulate(wt_list.begin(), wt_list.end(), 0);

    const double L = 0.1;
    //const double du = L/static_cast<double>(M);

    std::vector<double> fu;

    for(int i = 0; i < 2*M; i++){
        //fu.push_back( -1.0/(2.0*du) + static_cast<double>(i)*1.0/L );
        fu.push_back( 0.0 + static_cast<double>(i)*1.0/(2*L) );
    }

    auto plot_data = std::make_shared<PlotData>();
    plot_data->SetPlotStyle(Renderer::PlotStyle::Curve);
    plot_data->SetTitle("FFT MTF");
    plot_data->SetXLabel("Spatial Frequency");
    plot_data->SetYLabel("MTF");

    DiffractivePSF *psf_analyzer = new DiffractivePSF(opt_sys);


    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->GetOpticalSpec()->GetFieldSpec()->GetField(fi);
        Eigen::MatrixXd psf = Eigen::MatrixXd::Zero(M,M);

        for(int wi = 0; wi < num_wvls; wi++) {
            double wvl = wvl_list[wi];
            double wt = wt_list[wi];
            psf_analyzer->CreateFromOpdTrace(opt_sys, fld, wvl, M, L);
            Eigen::MatrixXd psf_for_wvl = psf_analyzer->ConvertToMatrix();
            psf += wt*psf_for_wvl;
        }

        psf = psf/sum_wt;


        //Eigen::MatrixXd psf2 = psf.array().pow(2);
        //Eigen::MatrixXcd psf2_c = psf2.cast<std::complex<double>>();

        //Eigen::MatrixXcd psf2_c = (psf.array().pow(2)). template cast<std::complex<double>>();

        Eigen::MatrixXcd temp = Eigen::MatrixXcd::Zero(2*M, 2*M);
        //temp.block(M, M,M,M) = psf2_c;
        temp.block(M, M,M,M) = (psf.array().pow(2)). template cast<std::complex<double>>();

        //Eigen::MatrixXcd temp1 = fftshift(temp);
        //Eigen::MatrixXcd temp2 = MatrixTool::fft2(fftshift(temp));
        //Eigen::MatrixXd temp3 = temp2.array().abs();
        //Eigen::MatrixXd temp3 = (MatrixTool::fft2(fftshift(temp))).array().abs();
        //double mtf0 = temp3(0,0);
        //Eigen::MatrixXd mtf = temp3.array()/mtf0;

        Eigen::MatrixXd mtf = (MatrixTool::fft2(fftshift(temp))).array().abs();
        double mtf0 = mtf(0,0);
        mtf = mtf.array()/mtf0;

        //std::vector<double> mtf_tan = MatrixTool::to_std_vector(mtf.col(0));
        //std::vector<double> mtf_sag = MatrixTool::to_std_vector(mtf.row(0));

        auto graph_sag = std::make_shared<Graph2d>();
        graph_sag->SetData(fu,MatrixTool::to_std_vector(mtf.row(0)));
        graph_sag->SetLineStyle(Renderer::LineStyle::Solid);
        graph_sag->SetRenderColor(fld->RenderColor());
        graph_sag->SetName("MTF_S F" + std::to_string(fi));

        auto graph_tan = std::make_shared<Graph2d>();
        graph_tan->SetData(fu,MatrixTool::to_std_vector(mtf.col(0)));
        graph_tan->SetLineStyle(Renderer::LineStyle::Dots);
        graph_tan->SetRenderColor(fld->RenderColor());
        graph_tan->SetName("MTF_T F" + std::to_string(fi));

        plot_data->AddGraph(graph_sag);
        plot_data->AddGraph(graph_tan);

    }

    delete psf_analyzer;


    return plot_data;
}

