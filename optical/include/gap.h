#ifndef GAP_H
#define GAP_H

#include <memory>

#include "medium.h"

namespace geopter {


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
    Gap(double t, std::shared_ptr<Medium> med);
    ~Gap();

    double thi() const;
    Medium* medium() const;

    void set_thi(double t);
    void set_medium(std::shared_ptr<Medium> med);

private:
    double thi_;
    std::shared_ptr<Medium> medium_;
};

} //namespace geopter

#endif // GAP_H
