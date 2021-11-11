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

//============================================================================
/// \file   glass_catalog.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#ifndef GLASSCATALOG_H
#define GLASSCATALOG_H

#include <memory>
#include <vector>
#include <string>

#include "glass.h"

namespace geopter {

class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    /**
     * @brief get glass property data from AGF file
     * @param agf_path AGF file path
     * @return success
     */
    bool load_agf(std::string agf_path);

    /** Get glass object pointer.  If not found, return nullptr */
    std::shared_ptr<Glass> glass(std::string product_name);

    /** Return glass ptr at the index */
    std::shared_ptr<Glass> glass(int i);

    /** Get supplier's name of the catalog */
    std::string name() const;

    /** Return number of glasses */
    int glass_count() const;

    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    /** supplier name */
    std::string name_;
    std::vector< std::shared_ptr<Glass> > glasses_;
};

} //namespace geopter


#endif // GLASSCATALOG_H
