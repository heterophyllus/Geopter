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
#include "system/optical_system.h"
#include "sequential/sequential_trace.h"

using namespace geopter;

OpticalSpec::OpticalSpec(OpticalSystem* opt_sys) :
    parent_(opt_sys)
{
    wavelength_spec_ = std::make_unique<WavelengthSpec>();
    pupil_           = std::make_unique<PupilSpec>();
    field_spec_      = std::make_unique<FieldSpec>();
}

OpticalSpec::~OpticalSpec()
{
    wavelength_spec_.reset();
    pupil_.reset();
    field_spec_.reset();
}


void OpticalSpec::CreateMinimumSpec()
{
    field_spec_->clear();
    field_spec_->AddField(0.0, 0.0, 1.0, rgb_black);

    wavelength_spec_->clear();
    wavelength_spec_->AddWavelength(SpectralLine::d, 1.0, rgb_black);

    pupil_->SetPupilType(PupilType::EPD);
    pupil_->SetValue(10);
}

void OpticalSpec::Clear()
{
    wavelength_spec_->clear();
    field_spec_->clear();
}

void OpticalSpec::update()
{
    // update object coords
    Eigen::Vector3d obj_pt;
    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    const int field_type = field_spec_->FieldType();
    double enp_dist = parent_->GetFirstOrderData()->entrance_pupil_distance;
    double obj_dist = parent_->GetFirstOrderData()->object_distance;
    double red      = parent_->GetFirstOrderData()->reduction;

    for(int fi = 0; fi < field_spec_->NumberOfFields(); fi++){
        Field* fld = field_spec_->GetField(fi);
        double fld_x = fld->X();
        double fld_y = fld->Y();

        switch (field_type)
        {
        case FieldType::OBJ_ANG:
            ang_dg = Eigen::Vector3d({fld_x, fld_y, 0.0});
            dir_tan(0) = tan(ang_dg(0) * M_PI/180.0);
            dir_tan(1) = tan(ang_dg(1) * M_PI/180.0);
            dir_tan(2) = tan(ang_dg(2) * M_PI/180.0);
            //obj_pt = -dir_tan*(fod_.obj_dist + fod_.enp_dist);
            obj_pt = -dir_tan*(obj_dist + enp_dist);
            break;

        case FieldType::OBJ_HT:
            obj_pt(0) = fld_x;
            obj_pt(1) = fld_y;
            obj_pt(2) = 0.0;
            break;

        case FieldType::IMG_HT:
            img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
            //obj_pt = fod_.red*img_pt;
            obj_pt = red*img_pt;
            break;

        default:
            obj_pt = Eigen::Vector3d::Zero(3);
        }

        fld->SetObjectPt(obj_pt);
    }

    // update aim pt
    if(parent_->GetOpticalAssembly()->NumberOfSurfaces() > 2)
    {
        SequentialTrace tracer(parent_);
        tracer.SetApplyVig(true);
        tracer.SetApertureCheck(false);

        Eigen::Vector2d aim_pt;
        Eigen::Vector3d obj_pt;
        double ref_wvl = wavelength_spec_->ReferenceWavelength();
        for(int fi = 0; fi < field_spec_->NumberOfFields(); fi++){
            Field* fld = field_spec_->GetField(fi);

            if(tracer.AimChiefRay(aim_pt, obj_pt, fld, ref_wvl)){
                field_spec_->GetField(fi)->SetAimPt(aim_pt);
                field_spec_->GetField(fi)->SetObjectPt(obj_pt);
            }else{
                std::cerr << "Ray aiming failed at field " << fi << std::endl;
                continue;
            }
        }
    }
}


void OpticalSpec::print(std::ostringstream &oss)
{
    oss << "Pupil Specs..." << std::endl;
    pupil_->Print(oss);

    oss << "Wavelengths..." << std::endl;
    wavelength_spec_->print(oss);

    oss << "Fields..." << std::endl;
    field_spec_->print(oss);

}
