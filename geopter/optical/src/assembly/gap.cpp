/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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


#include "assembly/gap.h"

using namespace geopter;

Gap::Gap()
{
    thi_ = 0.0;
    material_ = MaterialLibrary::GetAir();
}

Gap::Gap(double t, std::shared_ptr<Material> m){
    thi_ = t;
    if(m){
        material_ = m;
    }else{
        material_ = MaterialLibrary::GetAir();
    }
}

Gap::~Gap()
{
    material_ = nullptr;
}

void Gap::SetMaterial(std::shared_ptr<Material> m)
{
    if(m){
        material_ = m;
    }else{
        material_ = MaterialLibrary::GetAir();
    }
}

bool Gap::HasSolve() const {
    if(solve_) return true;
    return false;
}

int Gap::SolveType() const {
    if(solve_){
        return solve_->GetSolveType();
    }else{
        return -1;
    }
}
