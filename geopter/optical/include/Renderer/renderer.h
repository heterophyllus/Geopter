#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Eigen/Core"
#include "rgb.h"


namespace geopter {


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    enum LineStyle
    {
        Solid,
        Dots
    };

    virtual void set_grid_layout(int rows, int cols);
    virtual void set_current_cell(int row, int col);

    virtual void draw_line(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style=0);
    virtual void draw_polyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0);
    virtual void draw_polyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style=0);
    virtual void draw_text(std::string str, const Rgb& color);
    virtual void draw_dots(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double dot_size);

    virtual void draw_x_axis(bool state= true);
    virtual void draw_y_axis(bool state= true);
    virtual void set_x_axis_range(double xmin, double xmax);
    virtual void set_y_axis_range(double ymin, double ymax);
    virtual void set_x_axis_label(std::string label);
    virtual void set_y_axis_label(std::string label);


    /**
     * @brief Set aspect ratio h:v
     * @param ratio aspect ratio h/v
     */
    virtual void set_aspect_ratio(double h_per_v);

    virtual void update();

    virtual void clear();

protected:
    int current_cell_index_;
    int line_width_;
    int line_style_;
};

} //namespace geopter

#endif // RENDERER_H
