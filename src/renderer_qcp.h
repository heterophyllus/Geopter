#ifndef RENDERERQCP_H
#define RENDERERQCP_H

#include "renderer.h"
#include "qcustomplot.h"

using namespace geopter;


class RendererQCP : public Renderer
{
public:
    RendererQCP(QCustomPlot* customPlot);
    ~RendererQCP();

    void clear() override;
    void update() override;

    void set_grid_layout(int rows, int cols) override;
    void set_current_cell(int row, int col) override;

    void draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color, int line_style=0) override;
    void draw_polyline(std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0) override;
    void draw_polyline(std::vector<double>& x, std::vector<double>& y, const Rgb& color, int line_style=0) override;

    void set_x_axis_range(double xmin, double xmax) override;
    void set_y_axis_range(double ymin, double ymax) override;

    void set_x_axis_label(std::string label) override;
    void set_y_axis_label(std::string label) override;

    void set_aspect_ratio(double h_per_v) override;

private:
    QCustomPlot* customPlot_;

    inline QColor rgb_to_QColor(Rgb rgb);
};


QColor RendererQCP::rgb_to_QColor(Rgb rgb)
{
    return QColor(rgb.r*255.0, rgb.g*255.0, rgb.b*255.0);
}

#endif // RENDERERQCP_H
