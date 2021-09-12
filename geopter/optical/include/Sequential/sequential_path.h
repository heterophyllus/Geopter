#ifndef SEQUENTIALPATH_H
#define SEQUENTIALPATH_H

#include <vector>

namespace geopter {

class Surface;
class SurfaceProfile;

struct SequentialPathComponent
{
    SequentialPathComponent(){
        srf  = nullptr;
        d    = 0.0;
        rind = 1.0;
    }
    SequentialPathComponent(Surface* s, double thi, double n){
        srf  = s;
        d    = thi;
        rind = n;
    }
    ~SequentialPathComponent(){
        srf = nullptr;
    }

    Surface* srf;
    double d;
    double rind;
};


class SequentialPath
{
public:
    SequentialPath();
    ~SequentialPath();

    /** Returns number of path components */
    int size() const;

    /** Clear arrays */
    void clear();

    /** Append a new path component */
    void append(SequentialPathComponent seq_path_comp);

    /** Append a new path component */
    void append(Surface* s, double thi, double n);

    /** Access to path component at the given index */
    SequentialPathComponent at(int i) const;

private:
    std::vector<SequentialPathComponent> seq_path_comps_;
};

}

#endif // SEQUENTIALPATH_H
