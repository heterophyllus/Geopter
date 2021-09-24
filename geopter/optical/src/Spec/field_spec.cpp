#include "Spec/field_spec.h"

#include <iostream>
#include <iomanip>

using namespace geopter;

FieldSpec::FieldSpec(int field_type)
{
    field_type_ = field_type;
}

FieldSpec::~FieldSpec()
{
    clear();
}


void FieldSpec::set_field_type(int i)
{
    switch (i) {
    case FieldType::OBJ_ANG:
    case FieldType::OBJ_HT:
    case FieldType::IMG_HT:
        field_type_ = i;
        break;

    default:
        throw("Unknown field type");
    }
}

int FieldSpec::field_type() const
{
    return field_type_;
}

Field* FieldSpec::field(int i)
{
    if(i >= 0 && i < (int)fields_.size()){
        return fields_[i].get();
    }
    else if(i >= (int)fields_.size()){
        throw "Out of field range";
        return nullptr;
    }
    else{
        return fields_[fields_.size()+i].get();
    }
}

int FieldSpec::field_count()
{
    return (int)fields_.size();
}

void FieldSpec::add(double x, double y, double wt, Rgb color, double vuy, double vly, double vux, double vlx)
{
    auto fld = std::make_unique<Field>(x, y, wt, color, vuy, vly, vux, vlx);
    fields_.push_back(std::move(fld));
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

}


double FieldSpec::max_field()
{
    double max_fld_sqrd = 0.0;

    for(auto& f : fields_)
    {
        double fld_sqrd = f->x()*f->x() + f->y()*f->y();
        if(fld_sqrd > max_fld_sqrd)
        {
            max_fld_sqrd = fld_sqrd;
        }
    }

    return sqrt(max_fld_sqrd);
}

void FieldSpec::clear()
{
    if(!fields_.empty()){
        for(auto &f : fields_){
            f.reset();
        }
        fields_.clear();
    }
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
        oss << std::endl;
    }

    oss << std::endl;
}
