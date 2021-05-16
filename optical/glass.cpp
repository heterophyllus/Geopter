
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>


#include "glass.h"
#include "glass_catalog.h"
#include "spectral_line.h"
#include "dispersion_formula.h"


using namespace geopter;

Glass::Glass() : Medium(),
    formula_func_ptr_(nullptr)
{
    const int knum_coefs = 12;
    coefs_ = std::vector<double>(knum_coefs, 0.0);
}

Glass::~Glass()
{
    formula_func_ptr_ = nullptr;
    coefs_.clear();
}

std::string Glass::glass_code()
{
    std::string nd_part = std::to_string( (int)round((n_-1)*10000) );
    std::string vd_part = std::to_string( (int)round(v_*100) );
    return nd_part + "." + vd_part;
}


std::string Glass::display_name() const
{
    // ex. N-BK7_Schott
    return name_ + "_" + supplier_;
}

double Glass::rindex(double wv_nm) const
{
    return formula_func_ptr_(wv_nm/1000.0, coefs_);
}

double Glass::abbe_d() const
{
    double nd = rindex(SpectralLine::d);
    double nF = rindex(SpectralLine::F);
    double nC = rindex(SpectralLine::C);

    return (nd-1.0)/(nF-nC);
}

void Glass::set_dispersion_formula(int i)
{
    switch (i) {
    case 1:
        formula_func_ptr_ = &(DispersionFormula::Schott);
        break;
    case 2:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier1);
        break;
    case 3:
        formula_func_ptr_ = &(DispersionFormula::Herzberger);
        break;
    case 4:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier2);
        break;
    case 5:
        formula_func_ptr_ = &(DispersionFormula::Conrady);
        break;
    case 6:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier3);
        break;
    case 7:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics1);
        break;
    case 8:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics2);
        break;
    case 9:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier4);
        break;
    case 10:
        formula_func_ptr_ = &(DispersionFormula::Extended1);
        break;
    case 11:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier5);
        break;
    case 12:
        formula_func_ptr_ = &(DispersionFormula::Extended2);
        break;
    case 13: // Unknown
        formula_func_ptr_ = &(DispersionFormula::Unknown);
        break;
    default:
        formula_func_ptr_ = nullptr;
    }

}

void Glass::set_dispersion_coefs(int i, double val)
{
    if(i < (int)coefs_.size()){
        coefs_[i] = val;
    }else{
        return;
    }
}

void Glass::compute_refractive_index()
{
    if(formula_func_ptr_){
        n_ = formula_func_ptr_(SpectralLine::d, coefs_);
    }
}

void Glass::set_supplier(std::string sup)
{
    supplier_ = sup;
    std::transform(supplier_.begin(), supplier_.end(), supplier_.begin(), toupper);
}

std::string Glass::supplier() const
{
    return supplier_;
}

void Glass::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void Glass::print(std::ostringstream &oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    oss << "Glass: " << name_ << " (" << supplier_ << ")" << std::endl;
    oss << "nd: " << std::setw(val_w) << std::scientific << std::setprecision(prec) << n_ << std::endl;
    oss << "vd: " << std::setw(val_w) << std::scientific << std::setprecision(prec) << abbe_d() << std::endl;

    oss << "CD: ";
    for(int i = 0; i < (int)coefs_.size(); i++)
    {
        oss << std::scientific << std::setprecision(prec) << coefs_[i];
        oss << " ";
    }
    oss << std::endl;

}
