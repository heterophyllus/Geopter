#include "Paraxial/paraxial_path.h"

using namespace geopter;

ParaxialPath::ParaxialPath()
{
    if(!par_path_comps_.empty()){
        par_path_comps_.clear();
    }
}

ParaxialPath::~ParaxialPath()
{
    par_path_comps_.clear();
}

void ParaxialPath::prepend(ParaxialPathComponent par_path_comp)
{
    par_path_comps_.insert(par_path_comps_.begin(), par_path_comp);
}

void ParaxialPath::append(double c, double t, double n)
{
    par_path_comps_.push_back(ParaxialPathComponent(c,t,n));
}

void ParaxialPath::append(ParaxialPathComponent par_path_comp)
{
    par_path_comps_.push_back(par_path_comp);
}

int ParaxialPath::size() const
{
    return par_path_comps_.size();
}

ParaxialPathComponent ParaxialPath::at(int i) const
{
    return par_path_comps_[i];
}

ParaxialPathComponent ParaxialPath::front() const
{
    return par_path_comps_.front();
}

ParaxialPathComponent ParaxialPath::back() const
{
    return par_path_comps_.back();
}

void ParaxialPath::clear()
{
    par_path_comps_.clear();
}

