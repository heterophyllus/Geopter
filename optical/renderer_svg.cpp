#include <fstream>
#include <iostream>

#include "renderer_svg.h"


using namespace geopter;

RendererSvg::RendererSvg(int width, int height) :
    filename_("out.svg"),
    width_(width),
    height_(height),
    grid_rows_(1),
    grid_cols_(1),
    current_cell_row_(0),
    current_cell_col_(0)
{

}

RendererSvg::RendererSvg(std::string filename, double width, double height) :
    filename_(filename),
    width_(width),
    height_(height),
    grid_rows_(1),
    grid_cols_(1),
    current_cell_row_(0),
    current_cell_col_(0)
{

}

RendererSvg::~RendererSvg()
{
    std::ofstream file(filename_);
    write(file);
}

void RendererSvg::clear()
{
    out_.str("");
}

void RendererSvg::write(std::ostream &s)
{
    s << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;

    s << "<svg width=\"" << (width_) << "px\" height=\"" << (height_) << "px\" " << " viewbox=\"0,0,500,500\" "
      << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" "
         "xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
      << std::endl;

    // content
    s << out_.str ();

    s << "</svg>" << std::endl;
}


void RendererSvg::draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color)
{
    Eigen::Vector2i cp1 = convert(p1);
    Eigen::Vector2i cp2 = convert(p2);

    double cx1 = cp1(0);
    double cy1 = cp1(1);
    double cx2 = cp2(0);
    double cy2 = cp2(1);

    svg_begin_line(cx1, cy1, cx2, cy2);
    svg_add_stroke(color);
    svg_end();
}

void RendererSvg::draw_polyline(std::vector<Eigen::Vector2d> &pts, const Rgb& color)
{
    int count = pts.size();
    if(count < 3) return;

    out_ << "<polyline fill=\"none\"";
    svg_add_stroke(color);

    out_ << " points=\"";
    for(int i = 0; i < count; i++){
        Eigen::Vector2i cp = convert(pts[i]);
        out_ << cp(0) << "," << cp(1) << " ";
        //out_ << pts[i](0) << "," << pts[i](1) << " ";
    }
    out_ << "\" />" << std::endl;
}

void RendererSvg::draw_polyline(std::vector<double> &x, std::vector<double> &y, const Rgb& color)
{
    out_ << "<polyline fill=\"none\"";
    svg_add_stroke(color);

    int count = x.size();
    out_ << " points=\"";
    for(int i = 0; i < count; i++){
        Eigen::Vector2d p({x[i], y[i]});
        Eigen::Vector2i cp = convert(p);
        out_ << cp(0) << "," << cp(1) << " ";
        //out_ << x[i] << "," << y[i] << " ";
    }
    out_ << "\" />" << std::endl;

}

void RendererSvg::set_grid_layout(int rows, int cols)
{
    grid_rows_ = rows;
    grid_cols_ = cols;
}

void RendererSvg::set_current_cell(int row, int col)
{
    current_cell_row_ = row;
    current_cell_col_ = col;
}

void RendererSvg::set_x_axis_range(double xmin, double xmax)
{
    x_range_min_ = xmin;
    x_range_max_ = xmax;
}

void RendererSvg::set_y_axis_range(double ymin, double ymax)
{
    y_range_max_ = ymax;
    y_range_min_ = ymin;
}

void RendererSvg::draw_x_axis(bool state)
{
    if(state){
        Eigen::Vector2d p1({x_range_min_, 0});
        Eigen::Vector2d p2({x_range_max_, 0});
        this->draw_line(p1, p2, rgb_black);
    }
}

void RendererSvg::draw_y_axis(bool state)
{
    if(state){
        if(state){
            Eigen::Vector2d p1({0, y_range_min_});
            Eigen::Vector2d p2({0,y_range_max_});
            this->draw_line(p1, p2, rgb_black);
        }
    }
}

void RendererSvg::svg_add_stroke(const Rgb& rgb)
{
    out_ << " stroke=\"";
    write_srgb (rgb);
    out_ << "\"";
}

void RendererSvg::svg_begin_line(double x1, double y1, double x2, double y2, bool terminate)
{
    /*
    out_ << "<line "
           << "x1=\"" << (cx1) << "\" "
           << "y1=\"" << (cy1) << "\" "
           << "x2=\"" << (cx2) << "\" "
           << "y2=\"" << (cy2) << "\" ";

    */
    out_ << "<line "
           << "x1=\"" << (x1) << "\" "
           << "y1=\"" << (y1) << "\" "
           << "x2=\"" << (x2) << "\" "
           << "y2=\"" << (y2) << "\" ";


    if (terminate){
        out_ << " />" << std::endl;
    }
}

void RendererSvg::svg_begin_ellipse (double x, double y, double rx, double ry, bool terminate)
{
    out_ << "<ellipse "
           << "cx=\"" << x << "\" "
           << "cy=\"" << y << "\" "
           << "rx=\"" << rx << "\" "
           << "ry=\"" << ry << "\" ";

    if (terminate){
        out_ << " />" << std::endl;
    }
}

void RendererSvg::svg_begin_rect(double x1, double y1, double x2, double y2, bool terminate)
{
    out_ << "<rect "
           << "x=\"" << (x1) << "\" "
           << "y=\"" << (y1) << "\" "
           << "width=\"" << (x2 - x1) << "\" "
           << "height=\"" << (y2 - y1) << "\" ";

      if (terminate){
          out_ << " />" << std::endl;
      }
}

void RendererSvg::svg_add_fill (const Rgb &rgb)
{
    out_ << " fill=\"";
    write_srgb (rgb);
    out_ << "\"";
}

void RendererSvg::svg_end()
{
    out_ << " />" << std::endl;
}

void RendererSvg::write_srgb(const Rgb& rgb)
{
    char str[8];

    snprintf (str, 8, "#%02x%02x%02x", (unsigned char)(rgb.r * 255.0),
            (unsigned char)(rgb.g * 255.0), (unsigned char)(rgb.b * 255.0));

    out_ << str;
}



Eigen::Vector2i RendererSvg::convert(const Eigen::Vector2d& p)
{
    double cell_width = (double)width_/(double)grid_cols_;
    double cell_height = (double)height_/(double)grid_rows_;
    double x_range = x_range_max_ - x_range_min_;
    double y_range = y_range_max_ - y_range_min_;

    long int lcl_x = (long int)( cell_width*(p(0)-x_range_min_)/x_range );
    long int lcl_y = (long int)( cell_height - cell_height*(p(1)-y_range_min_)/y_range );

    Eigen::Vector2i cp;
    cp(0) = cell_width*current_cell_col_ + lcl_x;
    cp(1) = cell_height*current_cell_row_ + lcl_y;

    return cp;
}

