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

#include "spec/field.h"

#include <vector>
#include <iterator>
#include <string>
#include <memory>

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

    int NumberOfFields() { return num_fields_;}

    /** Returns current field type */
    int FieldType() const { return field_type_;}

    /** Access to field component */
    Field* GetField(int i) const { return fields_[i].get();}

    /** Returns the maximum field of view */
    double MaxField() const {return max_field_;}

    /**
     * @brief Set field type
     * @param int FieldType:OBJ_ANG, OBJ_HT, IMG_HT
     *  */
    void SetFieldType(int i) { field_type_ = i; }

    /** Add new field */
    void AddField(double x, double y, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);

    /** Remove field at the given index */
    void RemoveField(int i);

    void clear();

    void print();
    void print(std::ostringstream& oss);

protected:
    void update();

    int field_type_;
    std::vector< std::unique_ptr<Field> > fields_;
    double max_field_;

    int num_fields_;
};


} //namespace geopter


#endif // FIELDSPEC_H
