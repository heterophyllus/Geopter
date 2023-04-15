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


#ifndef ELEMENT_MODEL_H
#define ELEMENT_MODEL_H

#include <vector>
#include <memory>

#include "system/optical_system.h"
#include "element/element.h"

namespace geopter{


class ElementModel
{
public:
    ElementModel(OpticalSystem* sys);
    ~ElementModel();

    /** Returns pointer to element at the given index */
    Element* GetElement(int i) const;

    /** Returns number of elements */
    int NumberOfElements() const;

    /** Create elements list */
    void Create();

    /** Clear elements list */
    void Clear();

    
    void Update_Model();

private:
    OpticalSystem* opt_sys_;
    std::vector< std::unique_ptr<Element> > elements_;
};

}

#endif
