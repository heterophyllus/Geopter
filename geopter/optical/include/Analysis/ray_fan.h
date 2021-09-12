#ifndef RAYFAN_H
#define RAYFAN_H

namespace geopter {

class OpticalSystem;
class Renderer;

class RayFan
{
public:
    RayFan(OpticalSystem* opt_sys, Renderer* renderer);
    ~RayFan();

    /**
     * @brief Plot ray fans
     * @param yscale scale for y axis
     * @param nrd number of rays across diameter
     */
    void plot(double scale, int nrd=21);

private:
    OpticalSystem* opt_sys_;
    Renderer* renderer_;

    const int total_num_data_;
};

} //namespace geopter

#endif // RAYFAN_H
