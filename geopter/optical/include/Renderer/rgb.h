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

/* This file is a slightly modified version of rgb.h of Goptical Core library.
   The license notice of the original code is:
*/

/*
      This file is part of the Goptical Core library.
      The Goptical library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
      The Goptical library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public
      License along with the Goptical library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet
*/


//============================================================================
/// \file   rgb.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================




#ifndef RGB_H
#define RGB_H


namespace geopter {


/** Container for RGB color **/
struct Rgb
{
public:
    Rgb() : r(0.0), g(0.0), b(0.0), a(0.0){ }
    Rgb(double red, double green, double blue, double alpha = 0.0) : r(red), g(green), b(blue), a(alpha){ }

    double r;
    double g;
    double b;
    double a;
};


extern const Rgb rgb_black;
extern const Rgb rgb_red;
extern const Rgb rgb_green;
extern const Rgb rgb_blue;
extern const Rgb rgb_yellow;
extern const Rgb rgb_cyan;
extern const Rgb rgb_magenta;
extern const Rgb rgb_gray;
extern const Rgb rgb_white;

} //namespace geopter

#endif // RGB_H
