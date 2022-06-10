/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/


#ifndef FIELDSPEC_H
#define FIELDSPEC_H

#include <vector>
#include <iterator>
#include <string>
#include <memory>

#include "spec/field.h"

namespace geopter {

enum FieldType{
    OBJ_ANG,
    OBJ_HT,
    IMG_HT
};

class FieldSpec
{
public:
    FieldSpec();
    FieldSpec(int field_type);
    ~FieldSpec();

    static int number_of_fields() { return num_fields_;}

    /** Returns current field type */
    int field_type() const { return field_type_;}

    /** Get number of fields */
    int field_count() const { return fields_.size();}

    /** Access to field component */
    Field* field(int i) const { return fields_[i].get();}

    /** Returns the maximum field of view */
    double max_field() const {return max_field_;}

    /**
     * @brief Set field type
     * @param int FieldType:OBJ_ANG, OBJ_HT, IMG_HT
     *  */
    void set_field_type(int i);

    /** Add new field */
    void add(double x, double y, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);

    /** Remove field at the given index */
    void remove(int i);

    void clear();

    void print();
    void print(std::ostringstream& oss);

protected:
    void update();

    int field_type_;
    std::vector< std::unique_ptr<Field> > fields_;
    double max_field_;

    static int num_fields_;
};


} //namespace geopter


#endif // FIELDSPEC_H
