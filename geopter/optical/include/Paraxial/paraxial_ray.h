#ifndef PARAXIALRAY_H
#define PARAXIALRAY_H

#include <vector>
#include <string>
#include <sstream>

namespace geopter {

/** Paraxial ray parameter at each surface */
class ParaxialRayAtSurface
{
public:
    ParaxialRayAtSurface(){
        y_ = 0.0;
        u_prime_ = 0.0;
        i_ = 0.0;
        n_prime_ = 0.0;
        before_ = nullptr;
    }

    ParaxialRayAtSurface(double y, double u_prime, double i, double n_prime){
        y_ = y;
        u_prime_ = u_prime;
        i_ = i;
        n_prime_ = n_prime;
        before_ = nullptr;
    }

    void set_before(ParaxialRayAtSurface* before){
        before_ = before;
    }

    double y(){
        return y_;
    }

    /** Returns incoming slope */
    double u(){
        if(before_){
            return before_->u_prime();
        }else{ // assume at object
            return u_prime_;
        }
    }

    /** Returns outgoing slope */
    double u_prime(){
        return u_prime_;
    };

    double i(){
        return i_;
    }

    double n(){
        if(before_){
            return before_->n_prime();
        }else{
            return n_prime_;
        }
    }

    double n_prime(){
        return n_prime_;
    }

    /** Returns paraxial conjugate point distance */
    double l_prime() {
        return (- y_/u_prime_);
    }

private:
    double y_;
    double u_prime_; // outgoing
    double i_;       // incident
    double n_prime_; // outgoing

    ParaxialRayAtSurface *before_;
};


/** Paraxial ray */
class ParaxialRay
{
public:
    ParaxialRay();
    ~ParaxialRay();

    void prepend(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void prepend(double y, double u_prime, double i= 0, double n_prime = 1.0);
    
    void append(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void append(double y, double u_prime, double i= 0, double n_prime = 1.0);

    void clear();

    std::string name() const;
    void set_name(std::string name);

    /** Return number of components, usually equal to number of surfaces. */
    int size() const;

    /** Access to data at the given index */
    std::shared_ptr<ParaxialRayAtSurface> at(int i) const;

    /** Returns the data at the beginning */
    std::shared_ptr<ParaxialRayAtSurface> front() const;

    /** Return the data at the last surface */
    std::shared_ptr<ParaxialRayAtSurface> back() const;

    /** Access to the data at the surface just before the image */
    std::shared_ptr<ParaxialRayAtSurface> at_before_image() const;

    /** Write ray data to standard output */
    void print() const;

    /** Write property data to stream */
    void print(std::ostringstream& oss) const;

private:
    std::vector< std::shared_ptr<ParaxialRayAtSurface> > par_ray_at_srfs_;
    std::string name_;
};

} //namespace geopter

#endif // PARAXIALRAY_H
