/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
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
#include <iostream>
#include <map>

#include "assembly/surface.h"
#include "assembly/gap.h"
#include "solve/solve.h"
#include "common/string_tool.h"

namespace geopter {

class OpticalSystem;

/** Class to contain the optical components of the optical system */
class OpticalAssembly
{
public:
    OpticalAssembly(OpticalSystem* opt_sys);
    ~OpticalAssembly();

    void Clear();

    template<typename ... A>
    void SetupFromText(A... args);

    int NumberOfSurfaces() { return num_surfs_;}

    /** Get surface at the given index */
    Surface* GetSurface(int i) const { return interfaces_[i].get();}

    /** Get gap at the given index */
    Gap* GetGap(int i) const {return gaps_[i].get();}

    Surface* CurrentSurface() const {return interfaces_[current_surface_index_].get();}

    Gap* CurrentGap() const {return gaps_[current_surface_index_].get();}


    /** Returns number of gaps */
    int NumberOfGaps() const { return gaps_.size();}

    /** Returns current index of the stop */
    int StopIndex() const { return stop_index_;}

    /** Returns surface at the stop index */
    Surface* StopSurface() const { return interfaces_[stop_index_].get();}

    /** Returns the index of the image surface */
    int ImageIndex() const {return interfaces_.size()-1;}

    /** Returns image surface pointer */
    Surface* ImageSurface() const {return interfaces_[interfaces_.size()-1].get();}

    /** Returns the last gap */
    Gap* ImageSpaceGap() const;

    /** Set the given surface as stop */
    void SetStop(int i) { stop_index_ = i;}

    void CreateMinimumAssembly();

    /** insert a dummy surface */
    void Insert(int i);

    void Insert(int i, double r, double t, const std::string& mat_name="AIR");

    /** Remove surface and gap from sequence model */
    void Remove(int i);

    /** Compute and update global surface coordinates with reference to @param{ref_srf} */
    void SetGlobalTransforms(int ref_srf=1);

    /** Compute and update forward surface coordinates (r.T, t) for each interface */
    void SetLocalTransforms();

    void UpdateTransforms();

    void UpdateSolve();

    void UpdateSemiDiameters();

    /** Returns overall length from start to end */
    double OverallLength(int start, int end);


    /** List up model properties */
    void Print(std::ostringstream& oss) const;
    void Print() const;

private:
    OpticalSystem* parent_;

    std::vector< std::unique_ptr<Surface> > interfaces_;
    std::vector< std::unique_ptr<Gap> > gaps_;

    int stop_index_;
    int current_surface_index_;

    int num_surfs_;
    int num_gaps_;

};

template<typename... A>
void OpticalAssembly::SetupFromText(A... args)
{
    /* lens data must be commma separated text
     *
     * 1,   190.7535,  3.000,  1.6937:53.3,
     * 2,    18.8098,  9.500,
     * 3,    51.5630,  2.9,    1.6937:53.3,
     * 4, ...
     * ...
     * 27,  154.8320,  BF
     *
     */

    this->Clear();

    // add obejct surface
    this->Insert(0);

    int count = 0;
    for(auto s : std::initializer_list<std::string>{args...}) {
        count++;
        std::vector<std::string> srtm = StringTool::Split(s, ',');

        int si = 0;
        double r = 0;
        double t = 0;

        try{
            si = std::stoi(srtm[0]);
            r = std::stod(srtm[1]);
            t = std::stod(srtm[2]);
        }catch(...){
            std::cerr << "Failed to parse line (" << count << ")" << std::endl;
            this->Insert(this->ImageIndex()+1, std::numeric_limits<double>::infinity(), 0.0, "AIR");
            continue;
        }

        std::string mat_name = "AIR";

        if(srtm.size() > 3){
            mat_name = srtm[3];
        }

        this->Insert(si, r, t, mat_name);
    }

    // add image surface
    this->Insert(this->ImageIndex() + 1, std::numeric_limits<double>::infinity(), 0.0, "AIR");

}

}

#endif // OPTICALASSEMBLY_H
