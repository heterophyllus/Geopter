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


#ifndef SYSTEM_LAYOUT_H
#define SYSTEM_LAYOUT_H

#include "system/optical_system.h"
#include "assembly/surface.h"
#include "element/lens.h"
#include "element/dummy_interface.h"
#include "element/stop.h"
#include "sequential/ray.h"
#include "renderer/renderer.h"
#include "renderer/rgb.h"

namespace geopter{

/** Layout class support 2d drawing of optical system including optical elements, rays and other components. */
class Layout
{
public:
    Layout(OpticalSystem* sys, Renderer* renderer);
    ~Layout();

    void draw_elements();

    /** Draw chief ray, upper/lower marginal rays for one field */
    void draw_reference_rays();

    void draw_fan_rays(int nrd= 10);

    /** Draw a single ray */
    void draw_single_ray(const std::shared_ptr<Ray>& ray, const Rgb& color);

    void update();

private:
    OpticalSystem* opt_sys_;
    Renderer* renderer_;

    void draw_lens(Lens* lens, const Rgb& color);
    void draw_surface(Surface* srf, double max_y, const Rgb& color);
    void draw_stop(Stop* stop_elem, const Rgb &color);
    void draw_dummy_surface(DummyInterface *dummy_srf, const Rgb &color);
    void draw_flat(Surface* srf, double min_y, double max_y, const Rgb& color);
};

} //namespace geopter

#endif // LAYOUT_H
