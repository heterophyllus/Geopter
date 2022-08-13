#include "paraxial/first_order_data.h"

#include "system/optical_system.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;

FirstOrderData::FirstOrderData(OpticalSystem* parent)
{
    parent_ = parent;
}

FirstOrderData::~FirstOrderData()
{
    parent_ = nullptr;
}


void FirstOrderData::update()
{
    ParaxialTrace tracer(parent_);

    const int img = parent_->optical_assembly()->image_index();
    const int last_surf = parent_->optical_assembly()->image_index() -1;
    const int stop = parent_->optical_assembly()->stop_index();
    const double ref_wvl = parent_->optical_spec()->spectral_region()->reference_wvl();
    const double n_0 = parent_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    const double n_k = parent_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl);


    /**************************************
     * get starting y, u, ybar, ubar
     * ************************************/
    Eigen::Vector2d y_nu1_p(1.0, 0.0);
    Eigen::Vector2d y_nu1_q(0.0, 1.0/n_0);
    Eigen::Vector2d y_nuk_p;
    Eigen::Vector2d y_nuk_q;
    Eigen::Vector2d y_nus_p;
    Eigen::Vector2d y_nus_q;
    Eigen::Matrix2d Mk = tracer.system_matrix(1,last_surf, ref_wvl);
    Eigen::Matrix2d Ms = tracer.system_matrix(1, stop, ref_wvl);

    y_nuk_p = Mk*y_nu1_p;
    y_nuk_q = Mk*y_nu1_q;
    y_nus_p = Ms*y_nu1_p;
    y_nus_q = Ms*y_nu1_q;

    double as1 = y_nus_p(0);
    double bs1 = y_nus_q(0);
    double ck1 = y_nuk_p(1);
    double dk1 = y_nuk_q(1);

    const double thi0 = parent_->optical_assembly()->gap(0)->thi();
    obj_dist = thi0;
    red = dk1 + thi0*ck1;

    double ybar1 = -bs1;
    double ubar1 = as1;

    enp_dist = -ybar1/(n_0*ubar1);

    double obj2enp_dist = thi0 + enp_dist;

    double y0 = 0.0;
    double u0 = 1.0;
    double uk;

    PupilSpec* pupil = parent_->optical_spec()->pupil_spec();
    switch (pupil->pupil_type())
    {
    case PupilType::EPD:
        u0 = 0.5 * pupil->value() / obj2enp_dist;
        break;
    case PupilType::NAO:
        u0 = n_0*tan(asin(pupil->value()/n_0));
        break;
    case PupilType::FNO:
        uk = -1.0/(2.0*pupil->value());
        u0 = uk/red;
        break;
    case PupilType::NA:
        uk = n_k*tan(asin(pupil->value()/n_k));
        u0 = uk/red;
        break;
    default:
        std::cout << "Invalid pupil type" << std::endl;
        break;
    }

    double ybar0 = 1.0;
    double ubar0 = 0.0;

    FieldSpec* fov = parent_->optical_spec()->field_of_view();
    double max_fld = fov->max_field();
    double ang;

    switch(fov->field_type()) {
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
            ybar0 = red*max_fld;
            ubar0 = -ybar0/obj2enp_dist;
            break;
        default:
            std::cout << "Invalid field type" << std::endl;
            break;
    }

    ref_u0 = u0;
    ref_y0 = y0;
    ref_ubar0 = ubar0;
    ref_ybar0 = ybar0;



    /****************************************************************
     * compute first order data
     * **************************************************************/

    auto ax_ray = tracer.trace_paraxial_ray_from_object(y0, u0, ref_wvl);
    auto pr_ray = tracer.trace_paraxial_ray_from_object(ybar0, ubar0, ref_wvl);

    double y1 = ax_ray->at(1)->y();
    double ubar0_prime = pr_ray->at(0)->u_prime();
    ybar1 = pr_ray->at(1)->y();
    double u0_prime = ax_ray->at(0)->u_prime();

    opt_inv = n_0 * ( y1*ubar0_prime - ybar1*u0_prime );
    efl = -1.0/ck1;
    fno = -1.0/(2.0*n_k*ax_ray->at(img)->u_prime());

    //obj_dist = parent_->optical_assembly()->gap(0)->thi();
    img_dist = parent_->optical_assembly()->image_space_gap()->thi();

    //red = dk1 + ck1*obj_dist;

    pp1 = (dk1 - 1.0)*(n_0/ck1);
    //fod->ppk = (p_ray->at(img-1)->y() - 1.0)*(n_k/ck1);
    ppk = (y_nuk_p(0)-1.0)*(n_k/ck1);
    ffl = pp1 - efl;
    bfl = efl - ppk;

    n_obj = n_0;
    n_img = n_k;

    img_ht = -opt_inv/(n_k*ax_ray->at(img)->u_prime());
    obj_ang = atan(pr_ray->at(0)->u_prime()) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray->at(0)->u_prime();
    enp_dist = -ybar1/nu_pr0;
    enp_radius = fabs(opt_inv/nu_pr0);
    if(std::isnan(enp_dist)){
        enp_dist = 1.0e+10;
    }
    if(std::isnan(enp_radius)){
        enp_radius = 1.0e+10;
    }

    exp_dist = -(pr_ray->at(img)->y()/pr_ray->at(img)->u_prime() - img_dist);
    exp_radius = fabs( opt_inv/(n_k*pr_ray->at(img)->u_prime()) );
    if(std::isnan(exp_dist)){
        exp_dist = -1.0e+10;
    }
    if(std::isnan(exp_radius)){
        exp_radius = -1.0e+10;
    }

    obj_na = n_0*sin( atan(ax_ray->at(0)->u_prime()) );
    img_na = n_k*sin( atan(ax_ray->at(img)->u_prime()) );

}

void FirstOrderData::print(std::ostringstream &oss)
{
    constexpr int fixed_w = 30;
    constexpr int pre = 4;

    oss << std::setw(fixed_w) << std::left << "Effective Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl << std::endl;

    oss << std::setw(fixed_w) << std::left << "Front Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl << std::endl;

    oss << std::setw(fixed_w) << std::left << "Back Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl << std::endl;

    oss << std::setw(fixed_w) << std::left << "F/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

    oss << std::setw(fixed_w) << std::left << "Reduction Rate";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Angle";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Height";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "Optical Invariant";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv << std::endl;

    oss << std::endl;
}
