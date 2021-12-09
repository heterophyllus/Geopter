#ifndef GEOPTER_RENDERER_GNUPLOT_H
#define GEOPTER_RENDERER_GNUPLOT_H

#include "renderer/renderer.h"
#include "matplot/matplot.h"

namespace geopter {

class RendererGnuplot : public Renderer
{
public:
    RendererGnuplot();

    void set_grid_layout(int rows, int cols) override;
    void set_current_cell(int row, int col) override;

    void plot(std::shared_ptr<PlotData> plot_data);
    void imshow(const Eigen::MatrixXd& Z);
    void show();

private:
    matplot::axes_handle axis_;

};

} // namespace geopter



#endif
