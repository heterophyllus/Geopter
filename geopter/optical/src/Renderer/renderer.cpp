#include "Renderer/renderer.h"

using namespace geopter;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{

}

void Renderer::draw_plot(std::shared_ptr<PlotData> plotdata)
{

}

void Renderer::draw_text(std::string str, const Rgb& color)
{

}

void Renderer::draw_line(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style)
{

}

void Renderer::draw_polyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style)
{

}

void Renderer::draw_polyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style)
{

}

void Renderer::draw_dots(const std::vector<double> &x, const std::vector<double> &y, const Rgb &color, double dot_size)
{

}

void Renderer::draw_x_axis(bool state)
{

}

void Renderer::draw_y_axis(bool state)
{

}

void Renderer::set_x_axis_range(double xmin, double xmax)
{

}

void Renderer::set_y_axis_range(double ymin, double ymax)
{

}


void Renderer::set_x_axis_label(std::string label)
{

}

void Renderer::set_y_axis_label(std::string label)
{

}

void Renderer::set_aspect_ratio(double h_per_v)
{

}

void Renderer::set_grid_layout(int rows, int cols)
{

}

void Renderer::set_current_cell(int row, int col)
{

}

void Renderer::clear()
{

}
