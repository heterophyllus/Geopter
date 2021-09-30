
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Material/glass.h"
#include "Material/glass_catalog.h"
#include "Material/dispersion_formula.h"

#include "Spec/spectral_line.h"

#include "Utility/utility.h"

using namespace geopter;

Glass::Glass() : Material(),
    formula_func_ptr_(nullptr)
{
    constexpr int knum_coefs = 12;
    coefs_ = std::vector<double>(knum_coefs, 0.0);
}

Glass::~Glass()
{
    formula_func_ptr_ = nullptr;
    coefs_.clear();
}


double Glass::rindex(double wv_nm) const
{
    if(formula_func_ptr_){
        return formula_func_ptr_(wv_nm/1000.0, coefs_);
    }else{
        return 1.0;
    }
}


std::string Glass::name() const
{
    return product_name_ + "_" + supplier_name_;
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
        if(Utility::contains(supplier_name_, "HIKARI")){
            formula_func_ptr_ = &(DispersionFormula::Nikon_Hikari);
        }else{
            formula_func_ptr_ = &(DispersionFormula::Unknown);
        }
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

std::string Glass::product_name() const
{
    return product_name_;
}

void Glass::set_product_name(std::string name)
{
    product_name_ = name;
    std::transform(product_name_.begin(), product_name_.end(), product_name_.begin(), toupper);
}

void Glass::set_supplier(std::string sup)
{
    supplier_name_ = sup;
    std::transform(supplier_name_.begin(), supplier_name_.end(), supplier_name_.begin(), toupper);
}

std::string Glass::supplier() const
{
    return supplier_name_;
}

double Glass::abbe_d() const
{
    double nd = rindex(SpectralLine::d);
    double nF = rindex(SpectralLine::F);
    double nC = rindex(SpectralLine::C);

    return (nd - 1.0)/(nF - nC);
}

void Glass::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void Glass::print(std::ostringstream &oss)
{
    //const int idx_w = 4;
    constexpr int val_w = 10;
    constexpr int prec  = 4;

    oss << "Glass: " << name_ << " (" << supplier_name_ << ")" << std::endl;
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
