#ifndef RENDERERSVG_H
#define RENDERERSVG_H

#include "renderer.h"
#include "Eigen/Core"

namespace geopter {

class RendererSvg : public Renderer
{
public:
    RendererSvg(int width = 500, int height = 500);
    RendererSvg(std::string filename, double width = 500, double height = 500);
    ~RendererSvg();

    void write(std::ostream &s);

    /** canvas size */
    void set_viewport(int w, int h);

    void clear() override;

    void set_grid_layout(int rows, int cols) override;
    void set_current_cell(int row, int col) override;

    void draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color) override;
    void draw_polyline(std::vector<Eigen::Vector2d>& pts, const Rgb& color) override;
    void draw_polyline(std::vector<double>& x, std::vector<double>& y, const Rgb& color) override;
    //void draw_text(std::string str, Rgb color=rgb_black) override;

    void draw_x_axis(bool state= true) override;
    void draw_y_axis(bool state= true) override;
    void set_x_axis_range(double xmin, double xmax) override;
    void set_y_axis_range(double ymin, double ymax) override;


private:
    void svg_add_stroke(const Rgb& rgb);
    void svg_add_fill (const Rgb &rgb);
    void svg_begin_line(double x1, double y1, double x2, double y2, bool terminate = false);
    void svg_begin_rect(double x1, double y1, double x2, double y2, bool terminate = false);
    void svg_begin_ellipse (double x, double y, double rx, double ry, bool terminate = false);
    void svg_end();
    void write_srgb(const Rgb& rgb);
    Eigen::Vector2i convert(const Eigen::Vector2d& p);


    /** contents */
    std::ostringstream out_;

    std::string filename_;
    int width_;
    int height_;

    double x_range_min_;
    double x_range_max_;
    double y_range_min_;
    double y_range_max_;

    int grid_rows_;
    int grid_cols_;
    int current_cell_row_;
    int current_cell_col_;
};

}

#endif // RENDERERSVG_H
