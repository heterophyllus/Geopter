#ifndef OPTICALASSEMBLY_H
#define OPTICALASSEMBLY_H

#include <vector>
#include <string>
#include <memory>

#include "Assembly/surface.h"
#include "Assembly/gap.h"

namespace geopter {


/** Class to contain the optical components of the optical system */
class OpticalAssembly
{
public:
    OpticalAssembly();
    ~OpticalAssembly();

    void clear();

    /** Get surface at the given index */
    Surface* surface(int i) const;

    /** Get gap at the given index */
    Gap* gap(int i) const;

    /** Returns number of surfaces */
    inline int surface_count() const;

    /** Returns number of gaps */
    inline int gap_count() const;

    /** Returns current index of the stop */
    inline int stop_index() const;

    /** Returns surface at the stop index */
    inline Surface* stop_surface() const;

    /** Returns the index of the image surface */
    inline int image_index() const;

    /** Returns image surface pointer */
    inline Surface* image_surface() const;

    /** Returns the last gap */
    Gap* image_space_gap() const;


    /** Set the given surface as stop */
    void set_stop(int i);

    /** Set object distance */
    void set_object_distance(double t0);

    void create_minimun_assembly();


    void add_surface_and_gap(std::shared_ptr<Surface> s, std::shared_ptr<Gap> g);

    /** insert a dummy surface */
    void insert_dummy(int i);

    /** Remove surface and gap from sequence model */
    void remove(int i);


    /** Compute and update global surface coordinates with reference to @param{ref_srf} */
    void set_global_transforms(int ref_srf=1);

    /** Compute and update forward surface coordinates (r.T, t) for each interface */
    void set_local_transforms();

    void update_model();

    /** Returns overall length from start to end */
    double overall_length(int start, int end);


    /** List up model properties */
    void print(std::ostringstream& oss) const;

private:
    std::vector< std::shared_ptr<Surface> > interfaces_;
    std::vector< std::shared_ptr<Gap> > gaps_;

    int stop_index_;
};

int OpticalAssembly::surface_count() const
{
    return interfaces_.size();
}

int OpticalAssembly::gap_count() const
{
    return gaps_.size() - 1;
}

int OpticalAssembly::stop_index() const
{
    return stop_index_;
}

int OpticalAssembly::image_index() const
{
    return interfaces_.size() - 1;
}

Surface* OpticalAssembly::image_surface() const
{
    return interfaces_.back().get();
}

Surface* OpticalAssembly::stop_surface() const
{
    return interfaces_[stop_index_].get();
}


}

#endif // OPTICALASSEMBLY_H
