#ifndef FIELDSPEC_H
#define FIELDSPEC_H

#include <vector>
#include <string>
#include <memory>

#include "field.h"

namespace geopter {

enum FieldType{
    OBJ_ANG,
    OBJ_HT,
    IMG_HT
};

class FieldSpec
{
public:
    FieldSpec(int field_type=1);
    ~FieldSpec();

    void set_field_type(int i);
    int field_type() const;

    Field* field(int i);

    void add_field(double x, double y, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);
    void remove(int i);

    /** Get number of fields */
    int field_count();

    /** calculates the maximum field of view */
    double max_field();

    void clear();

    void clear_vignetting();

    void print();
    void print(std::ostringstream& oss);

private:
    //std::shared_ptr<OpticalSpec> parent_optical_spec_;

    int field_type_;
    std::vector< std::unique_ptr<Field> > fields_;
};

} //namespace geopter


#endif // FIELDSPEC_H
