#ifndef FIELDSPEC_H
#define FIELDSPEC_H

#include <vector>
#include <string>
#include <memory>

#include "Spec/field.h"

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

    /** 
     * @brief Set field type
     * @param int FieldType:OBJ_ANG, OBJ_HT, IMG_HT
     *  */
    void set_field_type(int i);

    /** Returns current field type */
    int field_type() const;

    /** Access to field component */
    Field* field(int i);

    /** Add new field */
    void add(double x, double y, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);

    /** Remove field at the given index */
    void remove(int i);

    /** Get number of fields */
    int field_count();

    /** Calculates the maximum field of view */
    double max_field();


    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    //std::shared_ptr<OpticalSpec> parent_optical_spec_;

    int field_type_;
    std::vector< std::unique_ptr<Field> > fields_;
};

} //namespace geopter


#endif // FIELDSPEC_H
