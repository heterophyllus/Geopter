#include <iostream>
#include <iomanip>
#include <sstream>

#include "wvl_spec.h"

using namespace geopter;

WvlSpec::WvlSpec()
{
    reference_index_ = 0;
}

WvlSpec::~WvlSpec()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
}

Wvl* WvlSpec::wvl(int i)
{
    return wvls_[i].get();
}

int WvlSpec::wvl_count() const
{
    return (int)wvls_.size();
}

void WvlSpec::set_reference_index(int i)
{
    reference_index_ = i;
}

int WvlSpec::reference_index() const
{
    return reference_index_;
}

double WvlSpec::reference_wvl() const
{
    return wvls_[reference_index_]->value();
}

double WvlSpec::wavelength(int i) const
{
    return wvls_[i]->value();
}

void WvlSpec::add(double wl, double wt, Rgb render_color)
{
    auto w = std::make_unique<Wvl>(wl, wt, render_color);
    wvls_.push_back(std::move(w));
}

void WvlSpec::remove(int i)
{
    if(i >= (int)wvls_.size() || i < 0){
        return;
    }else{
        wvls_[i].reset();
        auto itr = wvls_.begin() + i;
        wvls_.erase(itr);
   }
}

void WvlSpec::clear()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
    reference_index_ = 0;
}

void WvlSpec::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void WvlSpec::print(std::ostringstream &oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;


    // header labels
    //oss << "Wavelengths:" << std::endl;
    oss << std::setw(idx_w) << std::right << "W";
    oss << std::setw(val_w) << std::right << "Value";
    oss << std::setw(val_w) << std::right << "Weight";
    oss << std::endl;

    // list up data
    int num_wvls = wvl_count();
    for(int i = 0; i < num_wvls; i++)
    {
        if(i == reference_index_){
            oss << std::setw(idx_w) << std::right << std::to_string(i) + "*";
        }else{
            oss << std::setw(idx_w) << std::right << i;
        }
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << wvl(i)->value();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << wvl(i)->weight();
        oss << std::endl;
    }
    oss << std::endl;

}
