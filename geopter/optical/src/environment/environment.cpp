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

#include "environment/environment.h"


using namespace geopter;

double Environment::temperature_ = 25;
double Environment::pressure_ = 101325.0;

Environment::Environment()
{
    temperature_ = 25;
    pressure_ = 101325.0;
}

void Environment::SetTemperature(double t)
{
    temperature_ = t;
}

void Environment::SetAirPressure(double p)
{
    pressure_ = p;
}

double Environment::Temperature()
{
    return temperature_;
}

double Environment::AirPressure()
{
    return pressure_;
}

