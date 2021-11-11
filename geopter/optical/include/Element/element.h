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

#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

#include "renderer/renderer.h"

namespace geopter {

/** Base class to describe optical element */
class Element
{
public:
    Element();
    virtual ~Element();

    virtual std::string label() const;

    virtual std::string element_type() const;

    virtual void draw(Renderer* renderer);

    //virtual double weight() const;
    //virtual double volume() const;

protected:
    std::string label_;
    std::string element_type_;
};

}

#endif
