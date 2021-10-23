#include "Common/geopter_error.h"

using namespace geopter;

OutOfRangeError::OutOfRangeError() :
    std::out_of_range("out of range error"),
    array_name_("Unknown array"),
    array_size_(0),
    access_index_(0)
{

}

OutOfRangeError::~OutOfRangeError()
{

}

std::string OutOfRangeError::what_str() const
{
    std::string str =  array_name_ + " out of range error: " + "(access to " + std::to_string(access_index_) + " in size " + std::to_string(array_size_);
    return str;
}

std::string OutOfRangeError::array_name() const
{
    return array_name_;
}

int OutOfRangeError::array_size() const
{
    return array_size_;
}

int OutOfRangeError::access_index() const
{
    return access_index_;
}

void OutOfRangeError::set_array_name(const std::string &name)
{
    array_name_ = name;
}

void OutOfRangeError::set_array_size(int n)
{
    array_size_ = n;
}

void OutOfRangeError::set_access_index(int i)
{
    access_index_ = i;
}


//**********************************************************************************************************

RayOutOfRangeError::RayOutOfRangeError() : OutOfRangeError(),
    ray_(nullptr)
{
    array_name_ = "Ray";
}

RayOutOfRangeError::~RayOutOfRangeError()
{
    ray_.reset();
}

std::shared_ptr<Ray> RayOutOfRangeError::ray() const
{
    return ray_;
}

void RayOutOfRangeError::set_ray(std::shared_ptr<Ray> ray)
{
    ray_ = ray;
}
