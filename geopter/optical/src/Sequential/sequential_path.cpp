#include <cassert>

#include "Sequential/sequential_path.h"
#include "Common/geopter_error.h"
#include "Spec/spectral_line.h"

using namespace geopter;

SequentialPath::SequentialPath() :
    wvl_(SpectralLine::d),
    array_size_(0)
{

}

SequentialPath::~SequentialPath()
{
    seq_path_comps_.clear();
}

int SequentialPath::size() const
{
    assert(array_size_ == (int)seq_path_comps_.size());
    return array_size_;
}

void SequentialPath::clear()
{
    seq_path_comps_.clear();
    array_size_ = 0;
}

void SequentialPath::append(SequentialPathComponent seq_path_comp)
{
    seq_path_comps_.push_back(seq_path_comp);
    array_size_ += 1;
}

void SequentialPath::append(Surface *s, double thi, double n)
{
    SequentialPathComponent seq_path_comp(s, thi, n);
    seq_path_comps_.push_back(seq_path_comp);
    array_size_ += 1;
}

SequentialPathComponent SequentialPath::at(int i) const
{
    assert(array_size_ == (int)seq_path_comps_.size());

    if(i < array_size_) {
        return seq_path_comps_[i];
    } else {
        throw OutOfRangeError();
    }
}

double SequentialPath::wvl() const
{
    return wvl_;
}

void SequentialPath::set_wvl(double wvl)
{
    wvl_ = wvl;
}

