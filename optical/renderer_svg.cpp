#include <fstream>
#include <iostream>

#include "renderer_svg.h"


using namespace geopter;

RendererSvg::RendererSvg(double width, double height) :
    filename_("out.svg"),
    width_(width),
    height_(height)
{
    clear();
}

RendererSvg::RendererSvg(std::string filename, double width, double height) :
    filename_(filename),
    width_(width),
    height_(height)
{
    clear();
}

RendererSvg::~RendererSvg()
{
    std::ofstream file(filename_);
    write(file);
}

void RendererSvg::clear()
{
    out_.str("");

    /*
    // background
    svg_begin_rect (0, 0, width_, height_);
    svg_add_fill (rgb_white);
    svg_end ();

    out_ << "<defs>" << std::endl;

    // dot shaped point
      out_ << "<g id=\""
           << "dot"
           << "\">" << std::endl;
      svg_begin_line (1, 1, 0, 0, true);
      out_ << "</g>" << std::endl;

      // cross shaped point
      out_ << "<g id=\""
           << "cross"
           << "\">" << std::endl;
      svg_begin_line (-3, 0, 3, 0, true);
      svg_begin_line (0, -3, 0, 3, true);
      out_ << "</g>" << std::endl;

      // square shaped point
      out_ << "<g id=\""
           << "square"
           << "\">" << std::endl;
      svg_begin_line (-3, -3, -3, 3, true);
      svg_begin_line (-3, 3, 3, 3, true);
      svg_begin_line (3, 3, 3, -3, true);
      svg_begin_line (3, -3, -3, -3, true);
      out_ << "</g>" << std::endl;

      // round shaped point
      out_ << "<g id=\""
           << "round"
           << "\">" << std::endl;
      svg_begin_ellipse (0, 0, 3, 3, false);
      out_ << " fill=\"none\" />";
      out_ << "</g>" << std::endl;

      // triangle shaped point
      out_ << "<g id=\""
           << "triangle"
           << "\">" << std::endl;
      svg_begin_line (0, -3, -3, 3, true);
      svg_begin_line (-3, 3, 3, 3, true);
      svg_begin_line (0, -3, +3, +3, true);
      out_ << "</g>" << std::endl;

      out_ << "</defs>" << std::endl;
      */
}

void RendererSvg::write(std::ostream &s)
{
    s << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;

    s << "<svg width=\"" << width_ << "px\" height=\"" << height_ << "px\" "
      << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" "
         "xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
      << std::endl;

    // content
    s << out_.str ();

    s << "</svg>" << std::endl;
}


void RendererSvg::draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color)
{
    svg_begin_line(p1(0), p1(1), p2(0), p2(1));
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
        Eigen::Vector2d cp = convert(pts[i]);
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
        Eigen::Vector2d cp = convert(p);
        out_ << cp(0) << "," << cp(1) << " ";
        //out_ << x[i] << "," << y[i] << " ";
    }
    out_ << "\" />" << std::endl;

}

void RendererSvg::svg_add_stroke(const Rgb& rgb)
{
    out_ << " stroke=\"";
    write_srgb (rgb);
    out_ << "\"";
}

void RendererSvg::svg_begin_line(double x1, double y1, double x2, double y2, bool terminate)
{
    Eigen::Vector2d p1({x1, y1});
    Eigen::Vector2d p2({x2, y2});

    Eigen::Vector2d cp1 = convert(p1);
    Eigen::Vector2d cp2 = convert(p2);

    double cx1 = cp1(0);
    double cy1 = cp1(1);
    double cx2 = cp2(0);
    double cy2 = cp2(1);

    out_ << "<line "
           << "x1=\"" << (cx1) << "\" "
           << "y1=\"" << (cy1) << "\" "
           << "x2=\"" << (cx2) << "\" "
           << "y2=\"" << (cy2) << "\" ";

    /*
    out_ << "<line "
           << "x1=\"" << (x1) << "\" "
           << "y1=\"" << (y1) << "\" "
           << "x2=\"" << (x2) << "\" "
           << "y2=\"" << (y2) << "\" ";
           */

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


void RendererSvg::set_view_box(double min_x, double min_y, double max_x, double max_y)
{
    bmin_(0) = min_x;
    bmin_(1) = min_y;
    bmax_(0) = max_x;
    bmax_(1) = max_y;
}

Eigen::Vector2d RendererSvg::convert(const Eigen::Vector2d& p)
{
    const double s = ( p(0) - bmin_(0) ) / ( bmax_(0) - bmin_(0) );
    const double t = ( p(1) - bmin_(1) ) / ( bmax_(1) - bmin_(1) );

    Eigen::Vector2d cp;
    cp(0) = s * width_;
    cp(1) = (1.0 - t) * height_;
    return cp;
}
