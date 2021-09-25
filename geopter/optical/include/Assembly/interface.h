#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <memory>

#include "Assembly/transformation.h"
#include "Assembly/aperture.h"
#include "Assembly/decenter_data.h"

#include "Assembly/surface_profile.h"

namespace geopter {

class Interface
{
public:
    Interface();
    virtual ~Interface();

    std::string interact_mode() const;

    std::string label() const;
    void set_label(std::string lbl);

    DecenterData* decenter() const;

    void set_profile(std::unique_ptr<SurfaceProfile> prf);
    SurfaceProfile* profile() const;

    //void set_delta_n(double dn);
    //virtual void set_optical_power(double pwr, double n_before, double n_after);

    void set_clear_aperture(std::unique_ptr<Aperture> ca);
    void set_edge_aperture(std::unique_ptr<Aperture> ea);

    /** Remove all clear apertures from the surface */
    void remove_clear_aperture();

    Aperture* clear_aperture() const;
    Aperture* edge_aperture() const;

    /** Return aperture shape name. If no aperture is set, returns "None" */
    std::string aperture_shape() const;

    /** Maximum size of the interface.
      * If an aperture is set, this function returns the aperture max dimension.
      * Otherwise, it returns valid semi-diameter.
      */
    virtual double max_aperture() const;


    /** Maximum valid semi-diameter where the reference rays pass */
    virtual double semi_diameter() const;

    virtual void set_semi_diameter(double sd);

    /** Returns true if the given point(x,y) is inside of aperture */
    bool point_inside(double x, double y) const;
    bool point_inside(const Eigen::Vector2d& pt) const;

    void set_local_transform(Transformation tfrm);
    void set_global_transform(Transformation tfrm);
    Transformation local_transform() const;
    Transformation global_transform() const;

    virtual void update();

protected:
    std::string label_;
    std::string interact_mode_;

    double semi_diameter_;

    /** Shape of the interface */
    std::unique_ptr<SurfaceProfile> profile_;

    std::unique_ptr<Aperture> edge_aperture_;
    std::unique_ptr<Aperture> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    /** transform to next interface */
    Transformation lcl_tfrm_;

    /** global transform against the reference interface */
    Transformation gbl_tfrm_;
};

} //namespace geopter

#endif // INTERFACE_H
