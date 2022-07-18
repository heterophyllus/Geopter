#ifndef PARAXIAL_SURFACE_H
#define PARAXIAL_SURFACE_H

#include <optional>

namespace geopter{

class ParaxialSurface
{
public:
    ParaxialSurface(double c, double d, double n_prime, ParaxialSurface* before= nullptr) :
        c_(c), d_(d), n_prime_(n_prime), before_(before){}

    double n() {
        return before_->n_prime();
    }
    double n_prime() {
        return n_prime_;
    }

    double d(){
        return d_;
    }

    double l(){
        return ( before_->l_prime() - before_->d() );
    }
    double l_prime(){
        double phi = (n_prime_ - n())*c_;
        return n_prime_/( phi + n()/l() );
    }

    double i(){
        return (u() + y()*c_);
    }
    double i_prime(){
        return i()*(n()/n_prime_);
    }

    double u(){
        if(u_) return u_.value();
        u_ = before_->u_prime();
        return u_.value();
    }
    double u_prime(){
        return i_prime() - y()*c_;
    }

    double y(){
        if(y_) return y_.value();
        y_ = before_->y() + before_->d()*before_->u_prime();
        return y_.value();
    }

    void reset(){
        y_.reset();
        u_.reset();
    }

    void set_before(ParaxialSurface* before) {
        before_ = before;
    }

    void set_u_y(double u, double y){
        u_ = u;
        y_ = y;
    }

private:
    double c_;
    double d_;
    double n_prime_;
    ParaxialSurface* before_;
    std::optional<double> y_;
    std::optional<double> u_;
};

} //namespace
#endif
