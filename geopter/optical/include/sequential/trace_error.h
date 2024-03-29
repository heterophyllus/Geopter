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
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/



#ifndef TRACE_ERROR_H
#define TRACE_ERROR_H

#ifndef TraceError
#define TraceError uint32_t
#define TRACE_SUCCESS             (0x0000)
#define TRACE_TIR_ERROR           (0x1000)
#define TRACE_MISSEDSURFACE_ERROR (0x2000)
#define TRACE_BLOCKED_ERROR       (0x2001)
#define TRACE_NOT_REACHED_ERROR   (0x2002)
#endif

#endif // TRACE_ERROR_H
