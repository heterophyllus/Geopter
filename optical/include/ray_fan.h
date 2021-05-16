#ifndef RAYFAN_H
#define RAYFAN_H

namespace geopter {


class OpticalModel;
class Renderer;

class RayFan
{
public:
    RayFan(OpticalModel* opt_model, Renderer* renderer);
    ~RayFan();

    /**
     * @brief Plot ray fans
     * @param yscale scale for y axis
     * @param nrd number of rays across diameter
     */
    void plot(double scale, int nrd=21);

private:
    OpticalModel* opt_model_;
    Renderer* renderer_;

    const int total_num_data_;
};

} //namespace geopter

#endif // RAYFAN_H
