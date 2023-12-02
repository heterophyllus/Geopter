#include "paraxial/first_order_data.h"

#include "system/optical_system.h"
#include "paraxial/paraxial_trace.h"
#include "project/project.h"

using namespace geopter;

FirstOrderData::FirstOrderData(OpticalSystem* parent)
{
    parent_ = parent;
}

FirstOrderData::~FirstOrderData()
{
    parent_ = nullptr;
}


void FirstOrderData::Update()
{
    ParaxialTrace tracer(parent_);

    const int img = parent_->GetOpticalAssembly()->ImageIndex();
    const int last_surf = parent_->GetOpticalAssembly()->ImageIndex() -1;
    const int stop = parent_->GetOpticalAssembly()->StopIndex();
    const double ref_wvl = parent_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    const double n_0 = parent_->GetOpticalAssembly()->GetGap(0)->GetMaterial()->RefractiveIndex(ref_wvl);
    const double n_k = parent_->GetOpticalAssembly()->ImageSpaceGap()->GetMaterial()->RefractiveIndex(ref_wvl);


    /**************************************
     * get starting y, u, ybar, ubar
     * ************************************/
    Eigen::Vector2d y_nu1_p(1.0, 0.0);
    Eigen::Vector2d y_nu1_q(0.0, 1.0/n_0);
    Eigen::Vector2d y_nuk_p;
    Eigen::Vector2d y_nuk_q;
    Eigen::Vector2d y_nus_p;
    Eigen::Vector2d y_nus_q;
    Eigen::Matrix2d Mk = tracer.SystemMatrix(1,last_surf, ref_wvl);
    Eigen::Matrix2d Ms = tracer.SystemMatrix(1, stop, ref_wvl);

    y_nuk_p = Mk*y_nu1_p;
    y_nuk_q = Mk*y_nu1_q;
    y_nus_p = Ms*y_nu1_p;
    y_nus_q = Ms*y_nu1_q;

    double as1 = y_nus_p(0);
    double bs1 = y_nus_q(0);
    double ck1 = y_nuk_p(1);
    double dk1 = y_nuk_q(1);

    const double thi0 = parent_->GetOpticalAssembly()->GetGap(0)->Thickness();
    object_distance = thi0;
    reduction = dk1 + thi0*ck1;

    double ybar1 = -bs1;
    double ubar1 = as1;

    entrance_pupil_distance = -ybar1/(n_0*ubar1);

    double obj2enp_dist = thi0 + entrance_pupil_distance;

    double y0 = 0.0;
    double u0 = 1.0;
    double uk;

    PupilSpec* pupil = parent_->GetOpticalSpec()->GetPupilSpec();
    switch (pupil->PupilType())
    {
    case PupilType::EPD:
        u0 = 0.5 * pupil->Value() / obj2enp_dist;
        break;
    case PupilType::NAO:
        u0 = n_0*tan(asin(pupil->Value()/n_0));
        break;
    case PupilType::FNO:
        uk = -1.0/(2.0*pupil->Value());
        u0 = uk/reduction;
        break;
    case PupilType::NA:
        uk = n_k*tan(asin(pupil->Value()/n_k));
        u0 = uk/reduction;
        break;
    default:
        std::cout << "Invalid pupil type" << std::endl;
        break;
    }

    double ybar0 = 1.0;
    double ubar0 = 0.0;

    FieldSpec* fov = parent_->GetOpticalSpec()->GetFieldSpec();
    double max_fld = fov->MaxField();
    double ang;

    switch(fov->FieldType()) {
        case FieldType::OBJ_ANG:
            ang = max_fld * M_PI/180.0;
            ubar0 = tan(ang);
            ybar0 = -ubar0 * obj2enp_dist;
            break;
        case FieldType::OBJ_HT:
            ybar0 = -max_fld;
            ubar0 = -ybar0/obj2enp_dist;
            break;
        case FieldType::IMG_HT:
            ybar0 = reduction*max_fld;
            ubar0 = -ybar0/obj2enp_dist;
            break;
        default:
            std::cout << "Invalid field type" << std::endl;
            break;
    }

    reference_u0 = u0;
    reference_y0 = y0;
    reference_ubar0 = ubar0;
    reference_ybar0 = ybar0;


    /****************************************************************
     * compute first order data
     * **************************************************************/

    auto ax_ray = tracer.TraceParaxialRayFromObject(y0, u0, ref_wvl);
    auto pr_ray = tracer.TraceParaxialRayFromObject(ybar0, ubar0, ref_wvl);

    double y1 = ax_ray->At(1).y;
    double ubar0_prime = pr_ray->At(0).u_prime;
    ybar1 = pr_ray->At(1).y;
    double u0_prime = ax_ray->At(0).u_prime;

    optical_invariant = n_0 * ( y1*ubar0_prime - ybar1*u0_prime );
    effective_focal_length = -1.0/ck1;
    fno = -1.0/(2.0*n_k*ax_ray->At(img).u_prime);

    //obj_dist = parent_->GetOpticalAssembly()->gap(0)->thi();
    image_distance = parent_->GetOpticalAssembly()->ImageSpaceGap()->Thickness();

    //red = dk1 + ck1*obj_dist;

    pp1 = (dk1 - 1.0)*(n_0/ck1);
    //fod->ppk = (p_ray->at(img-1)->y() - 1.0)*(n_k/ck1);
    ppk = (y_nuk_p(0)-1.0)*(n_k/ck1);
    front_focal_length = pp1 - effective_focal_length;
    back_focal_length = effective_focal_length - ppk;

    n_obj = n_0;
    n_img = n_k;

    image_height = -optical_invariant/(n_k*ax_ray->At(img).u_prime);
    object_angle = atan(pr_ray->At(0).u_prime) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray->At(0).u_prime;
    entrance_pupil_distance = -ybar1/nu_pr0;
    entrance_pupil_radius = fabs(optical_invariant/nu_pr0);
    if(std::isnan(entrance_pupil_distance)){
        entrance_pupil_distance = 1.0e+10;
    }
    if(std::isnan(entrance_pupil_radius)){
        entrance_pupil_radius = 1.0e+10;
    }

    exit_pupil_distance = -(pr_ray->At(img).y/pr_ray->At(img).u_prime - image_distance);
    exit_pupil_radius = fabs( optical_invariant/(n_k*pr_ray->At(img).u_prime) );
    if(std::isnan(exit_pupil_distance)){
        exit_pupil_distance = -1.0e+10;
    }
    if(std::isnan(exit_pupil_radius)){
        exit_pupil_radius = -1.0e+10;
    }

    object_space_na = n_0*sin( atan(ax_ray->At(0).u_prime) );
    image_space_na = n_k*sin( atan(ax_ray->At(img).u_prime) );

}

void FirstOrderData::Print(std::ostringstream &oss)
{
    constexpr int fixed_w = 30;
    int pre = ProjectSetting::GetDecimals();

    oss << std::setw(fixed_w) << std::left << "Effective Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << effective_focal_length << std::endl;

    oss << std::setw(fixed_w) << std::left << "Front Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << front_focal_length << std::endl;

    oss << std::setw(fixed_w) << std::left << "Back Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << back_focal_length << std::endl;

    oss << std::setw(fixed_w) << std::left << "F/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

    oss << std::setw(fixed_w) << std::left << "Reduction Rate";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << reduction << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << object_distance << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Angle";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << object_angle << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << image_distance << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Height";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << image_height << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << entrance_pupil_distance << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << entrance_pupil_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exit_pupil_distance << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exit_pupil_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "Optical Invariant";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << optical_invariant << std::endl;

    oss << std::endl;
}
