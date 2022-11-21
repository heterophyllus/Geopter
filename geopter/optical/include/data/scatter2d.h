#ifndef GEOPTER_SCATTER_2D_H
#define GEOPTER_SCATTER_2D_H

#include "data/graph2d.h"

namespace geopter {

class Scatter2d : public Graph2d
{
public:
    Scatter2d();
    Scatter2d(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color= rgb_black, int line_style=0, double line_width=1.0);
    ~Scatter2d();

    double ref_x() const { return ref_x_;}
    double ref_y() const { return ref_y_;}
    double rms_spot_size() const;
    double rms_spot_x() const;
    double rms_spot_y() const;
    double geo_spot_size() const;

    void set_ref_x(double x) { ref_x_ = x;}
    void set_ref_y(double y) { ref_y_ = y;}

private:
    double ref_x_;
    double ref_y_;
};


}

#endif //GEOPTER_SCATTER_2D_H
