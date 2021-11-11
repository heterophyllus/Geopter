/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/


#ifndef OPTICALASSEMBLY_H
#define OPTICALASSEMBLY_H

#include <vector>
#include <string>
#include <memory>

#include "assembly/surface.h"
#include "assembly/gap.h"

namespace geopter {


/** Class to contain the optical components of the optical system */
class OpticalAssembly
{
public:
    OpticalAssembly();
    ~OpticalAssembly();

    void clear();

    /** Get surface at the given index */
    inline Surface* surface(int i) const;

    /** Get gap at the given index */
    inline Gap* gap(int i) const;

    inline Surface* current_surface() const;

    inline Gap* current_gap() const;

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

    void create_minimun_assembly();

    void add_surface_and_gap();

    /** insert a dummy surface */
    void insert_surface(int i);

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
    void print() const;

private:
    std::vector< std::unique_ptr<Surface> > interfaces_;
    std::vector< std::unique_ptr<Gap> > gaps_;

    int stop_index_;
    int current_surface_index_;
};




Surface* OpticalAssembly::surface(int i) const
{
    return interfaces_[i].get();
}

Gap* OpticalAssembly::gap(int i) const
{
    return gaps_[i].get();
}

Surface* OpticalAssembly::current_surface() const
{
    return interfaces_[current_surface_index_].get();
}

Gap* OpticalAssembly::current_gap() const
{
    return gaps_[current_surface_index_].get();
}

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
