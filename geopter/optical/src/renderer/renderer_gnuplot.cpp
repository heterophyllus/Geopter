#include "renderer/renderer_gnuplot.h"


using namespace geopter;

RendererGnuplot::RendererGnuplot()
{

}

void RendererGnuplot::set_grid_layout(int rows, int cols)
{
    matplot::tiledlayout(rows, cols);
}

void RendererGnuplot::set_current_cell(int row, int col)
{
    current_cell_index_ = ncols_*row + col;
    axis_ = matplot::nexttile(current_cell_index_);
}

void RendererGnuplot::plot(std::shared_ptr<PlotData> plot_data)
{
    int plot_style = plot_data->plot_style();
    int ndata = plot_data->data_count();

    if(Renderer::PlotStyle::Curve == plot_style){
        for(int i = 0; i < ndata; i++){
            auto graph = plot_data->graph(i);

            auto p = matplot::plot(axis_, graph->x_data(), graph->y_data());

            // set color
            Rgb color = graph->render_color();
            float r = color.r;
            float g = color.g;
            float b = color.b;
            p->color({r,g,b});

            p->line_width(graph->line_width());
        }

    }
    else if(Renderer::PlotStyle::Scatter == plot_style){
        for(int i = 0; i < ndata; i++){
            auto graph = plot_data->graph(i);

            auto p = matplot::scatter(axis_, graph->x_data(), graph->y_data());

            // set color
            Rgb color = graph->render_color();
            float r = color.r;
            float g = color.g;
            float b = color.b;
            p->color({r,g,b});

            p->line_width(graph->line_width());
        }
    }
}

void RendererGnuplot::imshow(const Eigen::MatrixXd& Z)
{
    const int nrows = Z.rows();
    //const int ncols = Z.cols();
    std::vector< std::vector<double> > C;

    for(int i = 0; i < nrows; i++){
        Eigen::VectorXd eigen_vec = Z.row(i);
        std::vector<double> std_vec(&eigen_vec[0], eigen_vec.data()+eigen_vec.cols()*eigen_vec.rows());
        C.push_back(std_vec);
    }

    matplot::image(C, true);
    matplot::colorbar();
}

void RendererGnuplot::show()
{
    matplot::show();
}
