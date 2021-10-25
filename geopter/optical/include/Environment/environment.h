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
/// \file   environment.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


class Environment
{
public:
    Environment();

    /**
     * @brief Set room temperature
     * @param t temperature in Celcius
     */
    static void set_temperature(double t);

    /**
     * @brief Set air pressure
     * @param p air pressure in Pascal
     */
    static void set_air_pressure(double p);

    static double temperature();

    static double air_pressure();

private:
    static double temperature_;
    static double pressure_;
};

#endif // ENVIRONMENT_H
