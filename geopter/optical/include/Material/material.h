#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>


namespace geopter {

/** Constant refractive index medium */
class Material
{
public:
    Material(double nd=1.0, std::string name="");
    virtual ~Material();

    virtual std::string name() const;
    virtual void set_name(std::string name);

    /** Return refractive index at specified wavelength */
    virtual double rindex(double wv_nm) const;

protected:
    std::string name_;
    double n_;
};

} //namespace geopter

#endif // MATERIAL_H
