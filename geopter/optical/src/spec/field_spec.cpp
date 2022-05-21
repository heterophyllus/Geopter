/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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

#include "spec/field_spec.h"

#include <iostream>
#include <iomanip>

using namespace geopter;

FieldSpec::FieldSpec()
{
    field_type_ = FieldType::OBJ_ANG;
    max_field_ = 0.0;
}

FieldSpec::FieldSpec(int field_type)
{
    field_type_ = field_type;
    max_field_ = 0.0;
}

FieldSpec::~FieldSpec()
{
    clear();
}

void FieldSpec::set_field_type(int i)
{
    assert( i >= 0 && i < 3);
    field_type_ = i;
}

void FieldSpec::add(double x, double y, double wt, Rgb color, double vuy, double vly, double vux, double vlx)
{
    auto fld = std::make_unique<Field>(x, y, wt, color, vuy, vly, vux, vlx);
    fields_.push_back(std::move(fld));
    update();
}

void FieldSpec::remove(int i)
{
    if( i >= (int)fields_.size() || i < 0 ){
        return;
    }else{
        fields_[i].reset();

        auto itr = fields_.begin() + i;
        fields_.erase(itr);
    }
    update();
}


void FieldSpec::update()
{
    assert(!fields_.empty());

    double max_fld_sqrd = 0.0;

    for(auto& f : fields_)
    {
        double x = f->x();
        double y = f->y();
        double fld_sqrd = x*x + y*y;
        if(fld_sqrd > max_fld_sqrd)
        {
            max_fld_sqrd = fld_sqrd;
        }
    }

    max_field_ = sqrt(max_fld_sqrd);
}

void FieldSpec::clear()
{
    if(!fields_.empty()){
        for(auto &f : fields_){
            f.reset();
        }
        fields_.clear();
    }
    max_field_ = 0.0;
}


void FieldSpec::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void FieldSpec::print(std::ostringstream &oss)
{
    /*----- Field Type -----*/
    oss << "Field Type: ";
    switch (field_type_) {
    case FieldType::OBJ_ANG:
        oss << "Object Angle";
        break;
    case FieldType::OBJ_HT:
        oss << "Object Height";
        break;
    case FieldType::IMG_HT:
        oss << "Image Height";
        break;
    default:
        oss << "Unknown";
    }
    oss << std::endl;

    /*----- values -----*/
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    oss << std::setw(idx_w) << std::right << "F";
    oss << std::setw(val_w) << std::right << "X";
    oss << std::setw(val_w) << std::right << "Y";
    oss << std::setw(val_w) << std::right << "Weight";
    oss << std::setw(val_w) << std::right << "VUY";
    oss << std::setw(val_w) << std::right << "VLY";
    oss << std::setw(val_w) << std::right << "VUX";
    oss << std::setw(val_w) << std::right << "VLX";
    oss << std::setw(val_w) << std::right << "AimPtX";
    oss << std::setw(val_w) << std::right << "AimPtY";
    oss << std::endl;

    int num_flds = fields_.size();
    for(int i = 0; i < num_flds; i++)
    {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->x();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->y();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->weight();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->vuy();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->vly();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->vux();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->vlx();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->aim_pt()(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << field(i)->aim_pt()(1);
        oss << std::endl;
    }

    oss << std::endl;
}
