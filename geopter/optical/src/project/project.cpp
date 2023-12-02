/*******************************************************************************
** Geopter
** Copyright (C) 2023 Hiiragi
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
**             Date: October 9th, 2023
********************************************************************************/

#include "project/project.h"

using namespace geopter;

int ProjectSetting::decimals_ = 4;

ProjectSetting::ProjectSetting()
{
    decimals_ = 4;
}

void ProjectSetting::SetDecimals(int n)
{
    decimals_ = n;
}

int ProjectSetting::GetDecimals()
{
    return decimals_;
}

