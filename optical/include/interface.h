#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "transform.h"

namespace geopter {

class Aperture;
class DecenterData;
class SurfaceProfile;

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

    void set_delta_n(double dn);
    virtual void set_optical_power(double pwr, double n_before, double n_after);

    void set_clear_aperture(std::unique_ptr<Aperture> ca);
    void set_adge_aperture(std::unique_ptr<Aperture> ea);

    /** Remove all clear apertures from the surface */
    void remove_clear_aperture();

    Aperture* clear_aperture();
    Aperture* edge_aperture();

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

    void set_local_transform(Tfrm tfrm);
    void set_global_transform(Tfrm tfrm);
    Tfrm local_transform() const;
    Tfrm global_transform() const;

    virtual void update();

protected:
    std::string label_;
    std::string interact_mode_;
    double delta_n_;

    /** maximum valid size where rays pass */
    double semi_diameter_;

    /** Shape of the interface */
    std::unique_ptr<SurfaceProfile> profile_;

    std::unique_ptr<Aperture> edge_aperture_;
    std::unique_ptr<Aperture> clear_aperture_;

    std::unique_ptr<DecenterData> decenter_;

    /** transform to next interface */
    Tfrm lcl_tfrm_;

    /** global transform against the reference interface */
    Tfrm gbl_tfrm_;
};

} //namespace geopter

#endif // INTERFACE_H
