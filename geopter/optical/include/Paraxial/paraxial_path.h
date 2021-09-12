#ifndef PARAXIALPATH_H
#define PARAXIALPATH_H

#include <vector>

namespace geopter {

struct ParaxialPathComponent
{
    ParaxialPathComponent() {
        c = 0.0;
        t = 0.0;
        n = 1.0;
    }

    ParaxialPathComponent(double cv, double thi, double ind){
        c = cv;
        t = thi;
        n = ind;
    }

    /** surface curvature */
    double c;

    /** gap thickness */
    double t;

    /** refractive index */
    double n;
};


class ParaxialPath
{
public:
    ParaxialPath();
    ~ParaxialPath();

    void prepend(ParaxialPathComponent par_path_comp);

    void append(double c, double t, double n);
    void append(ParaxialPathComponent par_path_comp);

    int size() const;

    ParaxialPathComponent at(int i) const;
    ParaxialPathComponent front() const;
    ParaxialPathComponent back() const;

    void clear();

private:
    std::vector<ParaxialPathComponent> par_path_comps_;
};

}

#endif // PARAXIALPATH_H
