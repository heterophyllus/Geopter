#ifndef OPTICALASSEMBLY_H
#define OPTICALASSEMBLY_H

#include <vector>
#include <string>
#include <memory>

namespace geopter {

class OpticalSystem;
class Surface;
class Gap;

/** Class to contain the optical components of the optical system */
class OpticalAssembly
{
public:
    OpticalAssembly(OpticalSystem* opt_sys);
    ~OpticalAssembly();

    void clear();

    /** Get surface at the given index */
    Surface* surface(int i) const;

    /** Get gap at the given index */
    Gap* gap(int i) const;

    /** Returns number of surfaces */
    int surface_count() const;

    /** Returns number of gaps */
    int gap_count() const;

    /** Returns current index of the stop */
    int stop_index() const;

    /** Returns the index of the image surface */
    int image_index() const;

    /** Returns the last gap */
    Gap* image_space_gap() const;

    /** Set the given surface as stop */
    void set_stop(int i);

    /** Set object distance */
    void set_object_distance(double t0);

    void create_minimun_assembly();


    /** add surface and gap just before the image */
    void add_surface_and_gap(double r, double t, std::string mat_name);

    void add_surface_and_gap(std::shared_ptr<Surface> s, std::shared_ptr<Gap> g);

    /** insert a dummy surface */
    void insert_dummy(int i);

    /** Remove surface and gap from sequence model */
    void remove(int i);

    /** @brief Updates semi diameter for all surfaces 
     *  @note Uses reference rays
    */
    void set_semi_diameters();

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
    

    OpticalSystem *parent_opt_sys_;
    std::vector< std::shared_ptr<Surface> > interfaces_;
    std::vector< std::shared_ptr<Gap> > gaps_;

    int stop_surface_;
};

}

#endif // OPTICALASSEMBLY_H
