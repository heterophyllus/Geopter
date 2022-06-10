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

#ifndef GAP_H
#define GAP_H

#include <memory>
#include <string>

#include "material/material.h"
#include "material/material_library.h"
#include "assembly/solve.h"

namespace geopter {


/**
 * @brief Gap container class
 *
 * The gap class represents the space between 2 surfaces. It contains the
    media definition for the space and a (z) displacement between the
    adjacent surfaces.

    The most common use case is an optical system with surfaces centered on a
    common axis. The Gap structure implements this case in the simplest manner.
    More complicated transformations between surfaces are implemented using
    transformations associated with the surfaces themselves.
 */
class Gap
{
public:
    Gap(){
        Gap(0.0, MaterialLibrary::air());
    }
    Gap(double t, std::shared_ptr<Material> m = MaterialLibrary::air());
    ~Gap(){
        material_ = nullptr;
    }

    double thi() const { return thi_; }
    void set_thi(double t) { thi_ = t; }

    Material* material() const { return material_.get();}
    void set_material(std::shared_ptr<Material> m) { material_ = m; }

    void set_solve(std::unique_ptr<Solve> solve) { solve_ = std::move(solve);}
    void remove_solve() { solve_.reset(); }
    Solve* solve() const { return solve_.get();}

    bool has_solve() const;
    int solve_type() const;

private:
    double thi_;
    std::shared_ptr<Material> material_;
    std::unique_ptr<Solve> solve_;
};


} //namespace geopter

#endif // GAP_H
