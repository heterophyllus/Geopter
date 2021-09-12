#ifndef GAP_H
#define GAP_H

#include <memory>
#include <string>

namespace geopter {

class Material;

/**
 * @brief Gap container class
 *
 * The gap class represents the space between 2 surfaces. It contains the
    media definition for the space and a (z) displacement between the
    adjacent surfaces.

    The most common use case is an optical system with surfaces centered on a
    common axis. The Gap structure implements this case in the simplest manner.
    More complicated transformations between surfaces are implemented using
    transformations associated with the surfaces themselves.
 */
class Gap
{
public:
    Gap();
    Gap(double t, std::shared_ptr<Material> m);
    ~Gap();

    double thi() const;
    void set_thi(double t);

    Material* material() const;
    void set_material(const std::string& material_name);
    void set_material(std::shared_ptr<Material> m);

private:
    double thi_;
    std::shared_ptr<Material> material_;
};

} //namespace geopter

#endif // GAP_H
