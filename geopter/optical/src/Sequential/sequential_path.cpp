#include "Sequential/sequential_path.h"

using namespace geopter;

SequentialPath::SequentialPath()
{

}


SequentialPath::~SequentialPath()
{
    seq_path_comps_.clear();
}

int SequentialPath::size() const
{
    return seq_path_comps_.size();
}

void SequentialPath::clear()
{
    seq_path_comps_.clear();
}

void SequentialPath::append(SequentialPathComponent seq_path_comp)
{
    seq_path_comps_.push_back(seq_path_comp);
}

void SequentialPath::append(Surface *s, double thi, double n)
{
    SequentialPathComponent seq_path_comp(s, thi, n);
    seq_path_comps_.push_back(seq_path_comp);
}

SequentialPathComponent SequentialPath::at(int i) const
{
    assert(i >= 0);

    if(i < (int)seq_path_comps_.size()) {
        return seq_path_comps_[i];
    } else {
        throw "Out of index";
    }
}
