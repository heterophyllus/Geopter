/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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

//============================================================================
/// \file   paraxial_path.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include "paraxial/paraxial_path.h"

using namespace geopter;

ParaxialPath::ParaxialPath()
{
    if(!par_path_comps_.empty()){
        par_path_comps_.clear();
    }
}

ParaxialPath::~ParaxialPath()
{
    par_path_comps_.clear();
}

void ParaxialPath::prepend(ParaxialPathComponent par_path_comp)
{
    par_path_comps_.insert(par_path_comps_.begin(), par_path_comp);
}

void ParaxialPath::append(double c, double t, double n)
{
    par_path_comps_.push_back(ParaxialPathComponent(c,t,n));
}

void ParaxialPath::append(ParaxialPathComponent par_path_comp)
{
    par_path_comps_.push_back(par_path_comp);
}

int ParaxialPath::size() const
{
    return par_path_comps_.size();
}

ParaxialPathComponent ParaxialPath::at(int i) const
{
    return par_path_comps_[i];
}

ParaxialPathComponent ParaxialPath::front() const
{
    return par_path_comps_.front();
}

ParaxialPathComponent ParaxialPath::back() const
{
    return par_path_comps_.back();
}

void ParaxialPath::clear()
{
    par_path_comps_.clear();
}

