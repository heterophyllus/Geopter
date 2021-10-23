#ifndef GEOPTER_ERROR_H
#define GEOPTER_ERROR_H

#include <exception>
#include <stdexcept>
#include <string>
#include <memory>


namespace geopter {

class Ray;

class OutOfRangeError : public std::out_of_range
{
public:
    OutOfRangeError();
    virtual ~OutOfRangeError();

    virtual std::string what_str() const;

    std::string array_name() const;
    int array_size() const;
    int access_index() const;

    void set_array_name(const std::string& name);
    void set_array_size(int n);
    void set_access_index(int i);

protected:
    std::string array_name_;
    int array_size_;
    int access_index_;
};


class RayOutOfRangeError : public OutOfRangeError
{
public:
    RayOutOfRangeError();
    ~RayOutOfRangeError();

    std::shared_ptr<Ray> ray() const;

    void set_ray(std::shared_ptr<Ray> ray);

private:
    std::shared_ptr<Ray> ray_;
};


}

#endif
