#ifndef RGB_H
#define RGB_H


namespace geopter {


/** Container for RGB color **/
struct Rgb
{
public:
    Rgb() : r(0.0), g(0.0), b(0.0), a(0.0){ }
    Rgb(double red, double green, double blue, double alpha = 0.0) : r(red), g(green), b(blue), a(alpha){ }

    double r;
    double g;
    double b;
    double a;
};


extern const Rgb rgb_black;
extern const Rgb rgb_red;
extern const Rgb rgb_green;
extern const Rgb rgb_blue;
extern const Rgb rgb_yellow;
extern const Rgb rgb_cyan;
extern const Rgb rgb_magenta;
extern const Rgb rgb_gray;
extern const Rgb rgb_white;

} //namespace geopter

#endif // RGB_H
