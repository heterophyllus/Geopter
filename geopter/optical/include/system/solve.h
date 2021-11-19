#ifndef GEOPTER_SOLVE_H
#define GEOPTER_SOLVE_H

namespace geopter{

enum SolveType
{
    ParaxialImage,
    ReductionRatio
};

class Solve
{
public:

private:
    double value_;
};

class ThicknessSolve : public Solve
{
private:
    int gap_index_;
    double value_;
}

} //namespace geopter

#endif //GEOPTER_SOLVE_H
