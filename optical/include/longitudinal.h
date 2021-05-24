#ifndef LONGITUDINAL_H
#define LONGITUDINAL_H

#include <vector>

namespace geopter {

class OpticalModel;
class Renderer;

class Longitudinal
{
public:
    Longitudinal(OpticalModel* opt_model, Renderer* renderer);
    ~Longitudinal();

    void plot(double lsa_scale, double fie_scale);

    void plot_lsa(double scale);
    void plot_ast(double scale);
    void plot_distortion(double scale);

private:
    OpticalModel* opt_model_;
    Renderer* renderer_;

    const int num_data_;

    /** Get paraxial image distance for all wavelengths */
    std::vector<double> get_l_primes();
};

}

#endif // LONGITUDINAL_H
