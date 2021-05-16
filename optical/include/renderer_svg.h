#ifndef RENDERERSVG_H
#define RENDERERSVG_H

#include "renderer.h"
#include "Eigen/Core"

namespace geopter {

class RendererSvg : public Renderer
{
public:
    RendererSvg(double width = 800, double height = 600);
    RendererSvg(std::string filename, double width = 800, double height = 600);
    ~RendererSvg();

    void write(std::ostream &s);

    void clear() override;

    void draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color) override;
    void draw_polyline(std::vector<Eigen::Vector2d>& pts, const Rgb& color) override;
    void draw_polyline(std::vector<double>& x, std::vector<double>& y, const Rgb& color) override;
    //void draw_text(std::string str, Rgb color=rgb_black) override;

    void set_view_box(double min_x, double min_y, double max_x, double max_y);

private:
    void svg_add_stroke(const Rgb& rgb);
    void svg_add_fill (const Rgb &rgb);
    void svg_begin_line(double x1, double y1, double x2, double y2, bool terminate = false);
    void svg_begin_rect(double x1, double y1, double x2, double y2, bool terminate = false);
    void svg_begin_ellipse (double x, double y, double rx, double ry, bool terminate = false);
    void svg_end();
    void write_srgb(const Rgb& rgb);

    Eigen::Vector2d convert(const Eigen::Vector2d& p);

    std::ostringstream out_;
    std::string filename_;
    double width_;
    double height_;

    Eigen::Vector2d bmin_;
    Eigen::Vector2d bmax_;
    Eigen::Vector2d size_;
};

}

#endif // RENDERERSVG_H
