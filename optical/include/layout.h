#ifndef LAYOUT_H
#define LAYOUT_H

#include "rgb.h"
#include "ray.h"


namespace geopter{

class Renderer;
class Element;
class Lens;
class OpticalModel;
class Interface;
class Surface;
class DummyInterface;

/** Layout class support 2d drawing of optical system including optical elements, rays and other components. */
class Layout
{
public:
    Layout(OpticalModel* opt_model, Renderer* renderer);
    ~Layout();

    void draw_elements();
    void draw_stop(DummyInterface* stop_elem, const Rgb& color);

    /** Draw chief ray, upper/lower marginal rays for one field */
    void draw_reference_rays();

    void draw_fan_rays(int nrd= 10);

    /** Draw a single ray */
    void draw_single_ray(const Ray& ray, const Rgb& color);

    void update();

private:
    OpticalModel* opt_model_;
    Renderer* renderer_;

    void draw_lens(Lens* lens, const Rgb& color);
    void draw_surface(Surface* srf, double max_y, const Rgb& color);
    void draw_stop();
    void draw_dummy_surface();
    void draw_flat(Surface* srf, double min_y, double max_y, const Rgb& color);
};

} //namespace geopter

#endif // LAYOUT_H
