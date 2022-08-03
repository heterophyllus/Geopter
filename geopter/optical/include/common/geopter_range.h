#ifndef GEOPTER_RANGE_H
#define GEOPTER_RANGE_H

#include <cmath>

namespace geopter {

template<typename ValueType>
class GPTRange //Geopter Range
{
public:
    GPTRange(){
        lower_ = 0;
        upper_ = 0;
    }
    GPTRange(ValueType lower, ValueType upper){
        this->set_range(lower, upper);
    }

    void set_range(ValueType lower, ValueType upper) {
        if(lower < upper){
            lower_ = lower;
            upper_ = upper;
        }else{
            lower_ = upper;
            upper_ = lower;
        }
    }

    void set_lower(ValueType lower) { lower_ = lower;}
    void set_upper(ValueType upper) { upper_ = upper;}

    ValueType lower() const { return lower_; }
    ValueType upper() const { return upper_; }
    ValueType length() const { return abs(upper_ - lower_); }

    bool contains(ValueType val) const { return (lower_ <= val && val <= upper_);}

private:
    ValueType lower_;
    ValueType upper_;
};

}



#endif //GEOPTER_RANGE_H


