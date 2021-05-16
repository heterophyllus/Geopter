#ifndef MEDIUM_H
#define MEDIUM_H

#include <string>


namespace geopter {

/** Constant refractive index medium */
class Medium
{
public:
    Medium(double nd=1.0, std::string name="");
    virtual ~Medium();

    virtual std::string name() const;
    virtual void set_name(std::string name);

    virtual std::string display_name() const;
    virtual void set_display_name(std::string disp_name);

    /** Return refractive index at specified wavelength */
    virtual double rindex(double wv_nm) const;

protected:
    std::string name_;
    std::string display_name_;
    double n_;
};

} //namespace geopter

#endif // MEDIUM_H
