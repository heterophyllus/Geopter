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


#include "spec/optical_spec.h"

using namespace geopter;

OpticalSpec::OpticalSpec()
{
    spectral_region_ = std::make_unique<WvlSpec>();
    pupil_           = std::make_unique<PupilSpec>();
    field_of_view_   = std::make_unique<FieldSpec>();
}

OpticalSpec::~OpticalSpec()
{
    spectral_region_.reset();
    pupil_.reset();
    field_of_view_.reset();
}


void OpticalSpec::create_minimum_spec()
{
    field_of_view_->clear();
    field_of_view_->add(0.0, 0.0, 1.0, rgb_black);

    spectral_region_->clear();
    spectral_region_->add(SpectralLine::d, 1.0, rgb_black);

    pupil_->set_value(PupilType::EPD);
    pupil_->set_value(10);
}

void OpticalSpec::clear()
{
    spectral_region_->clear();
    field_of_view_->clear();
}


void OpticalSpec::print(std::ostringstream &oss)
{
    oss << "Pupil Specs..." << std::endl;
    pupil_->print(oss);

    oss << "Wavelengths..." << std::endl;
    spectral_region_->print(oss);

    oss << "Fields..." << std::endl;
    field_of_view_->print(oss);

}
