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
/// \file   lend.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include "element/lens.h"


using namespace geopter;

Lens::Lens(Surface* s1, Surface* s2, Gap* g) :
    s1_(s1),
    s2_(s2),
    gap_(g)
{
    element_type_ = "Lens";
}

Lens::~Lens()
{
    s1_ = nullptr;
    s2_ = nullptr;
    gap_ = nullptr;
}

Surface* Lens::surface1() const
{
    return s1_;
}

Surface* Lens::surface2() const
{
    return s2_;;
}

void Lens::draw(Renderer* renderer)
{

}

void Lens::draw_edge(Renderer* renderer)
{

}
