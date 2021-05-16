#include "sequential_model.h"
#include "optical_spec.h"
#include "wvl_spec.h"
#include "field_spec.h"
#include "pupil_spec.h"

#include "database.h"
#include "optical_model.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <iomanip>

#include "medium.h"
#include "air.h"
#include "glass.h"

#include "surface.h"
#include "gap.h"
#include "surface_profile.h"
#include "aperture.h"

#include "glass_catalog.h"

#include "trace.h"
#include "file_io.h"


using namespace geopter;

SequentialModel::SequentialModel(OpticalModel* opt_model) :
    optical_model_(opt_model)
{
    initialize_arrays();
}

SequentialModel::~SequentialModel()
{
    clear();
}

void SequentialModel::clear()
{
    if(!interfaces_.empty())
    {
        for(auto &s : interfaces_){
            s.reset();
        }
        interfaces_.clear();
    }

    if(!gaps_.empty())
    {
        for(auto &g : gaps_){
            g.reset();
        }
        gaps_.clear();
    }

    z_dirs_.clear();
    lcl_tfrms_.clear();
    gbl_tfrms_.clear();

    for(auto &rn : rndx_){
        rn.clear();
    }
    rndx_.clear();

    wvlns_.clear();
}

void SequentialModel::initialize_arrays()
{
    clear();

    // add object interface
    auto s_obj = std::make_shared<Surface>("Obj");
    interfaces_.push_back(s_obj);

    Tfrm tfrm;
    tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
    tfrm.transfer = Eigen::Vector3d::Zero(3);

    gbl_tfrms_.push_back(tfrm);
    lcl_tfrms_.push_back(tfrm);

    // add object gap
    auto air = std::make_shared<Air>();
    auto g = std::make_shared<Gap>(0.0, air);
    gaps_.push_back(g);

    z_dirs_.push_back(1);

    rndx_.push_back(std::vector<double>(1, 1.0)); //(num of elem, value)

    // interfaces are inserted after cur_surface
    cur_surface_ = 0;

    // add image interface
    auto s_img = std::make_shared<Surface>("Img");
    interfaces_.push_back(s_img);

    gbl_tfrms_.push_back(tfrm);
    lcl_tfrms_.push_back(tfrm);
    z_dirs_.push_back(1);
    rndx_.push_back(std::vector<double>(1, 1.0));
}

void SequentialModel::update_model(bool update_sd)
{
    auto osp = optical_model_->optical_spec();
    auto ref_wl = osp->spectral_region()->reference_index();

    wvlns_.clear();
    for(int i = 0; i < osp->spectral_region()->wvl_count(); i++){
        wvlns_.push_back(osp->spectral_region()->wavelength(i));
    }
    rndx_ = calc_ref_indices_for_spectrum(wvlns_);
    auto n_before = rndx_[0];

    z_dirs_.clear();
    double z_dir_before = 1;

    for(int i = 0; i < (int)interfaces_.size(); i++)
    {
        auto s = interfaces_[i];
        double z_dir_after = 1.0 * z_dir_before/abs(z_dir_before);

        if(s->interact_mode() == "reflect"){
            z_dir_after = -z_dir_after;
        }

        // leave rndx data unsigned, track change of sign using z_dir
        auto n_after = rndx_[i];
        if(z_dir_after < 0){
            for(int j = 0; j < (int)n_after.size(); j++){
                n_after[j] = - n_after[j];
            }
        }
        double delta_n = n_after[ref_wl] - n_before[ref_wl] ;
        s->set_delta_n(delta_n);
        n_before = n_after;

        z_dir_before = z_dir_after;
        z_dirs_.push_back(z_dir_after);

        // call update() on the surface interface
        s->update();
    }

    gbl_tfrms_ = compute_global_coords();
    lcl_tfrms_ = compute_local_transforms();

    osp->update_model();

    if(update_sd){
        this->set_semi_diameters();
    }
}

int SequentialModel::surface_count() const
{
    return interfaces_.size();
}

Surface* SequentialModel::surface(int n) const
{
    if(n >= 0 && n < (int)interfaces_.size()){
        return interfaces_[n].get();
    }
    else if(n >= (int)interfaces_.size()){
        throw "Out of surface range";
    }else{
        return interfaces_[interfaces_.size() + n].get();
    }
}

Surface* SequentialModel::current_surface() const
{
    return interfaces_[cur_surface_].get();
}


int SequentialModel::gap_count() const
{
    return (int)gaps_.size();
}

Gap* SequentialModel::gap(int n) const
{
    if(n >= 0 && n < (int)gaps_.size()){
        return gaps_[n].get();
    }
    else{
        int gap_size = gaps_.size();
        return gaps_[gap_size + n].get();
    }
}

double SequentialModel::z_dir(int n) const
{
    if(n >= 0){
        return z_dirs_[n];
    }
    else{
        int total_size = z_dirs_.size();
        return z_dirs_[total_size + n];
    }
}

Path SequentialModel::path(double wl, int start, int stop, int step) const
{
    if(wl < 0){
        wl = central_wavelength();
    }

    int gap_start;
    if(step < 0){
        gap_start = start-1;
    }
    else{
        gap_start = start;
    }

    int max_size = std::max({(int)interfaces_.size(),
                             (int)gaps_.size(),
                             (int)rndx_.size(),
                             (int)lcl_tfrms_.size(),
                             (int)z_dirs_.size()});
    if(stop < 0){
        stop = max_size;
    }

    PathComponent path_component;
    Path path;
    int path_itr_cnt = 0;

    int ifc_itr = start;
    int gap_itr = gap_start;
    int rndx_itr = start;
    int lcl_tfrm_itr = start;
    int z_dir_itr = start;

    for(int main_itr = start; main_itr < stop; main_itr+=step)
    {
        if(ifc_itr < (int)interfaces_.size()){
            path_component.srf = interfaces_[ifc_itr].get();
        }else{
            path_component.srf = nullptr;
        }
        ifc_itr += step;

        if(gap_itr < (int)gaps_.size()){
            path_component.gap = gaps_[gap_itr].get();
        }else{
            path_component.gap = nullptr;
        }
        gap_itr += step;

        if(rndx_itr < (int)rndx_.size()){
            if(path_component.gap){
                path_component.rndx = path_component.gap->medium()->rindex(wl);
            }else{
                path_component.rndx = 1.0;
            }
        }else{
            path_component.rndx = 1.0; // should be None
        }
        rndx_itr += step;

        if(z_dir_itr < (int)z_dirs_.size()){
            path_component.z_dir = z_dirs_[z_dir_itr];
        }else{
            path_component.z_dir = 1.0; // should be None
        }
        z_dir_itr += step;

        if(lcl_tfrm_itr < (int)lcl_tfrms_.size()){
            path_component.tfrm = lcl_tfrms_[lcl_tfrm_itr];
        }else{
            path_component.tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
            path_component.tfrm.transfer = Eigen::Vector3d::Zero(3);
        }
        lcl_tfrm_itr += step;

        path.push_back(path_component);

        path_itr_cnt++;
    }

    return path;
}

std::vector< std::vector<double> > SequentialModel::calc_ref_indices_for_spectrum(std::vector<double> wvl)
{
    std::vector< std::vector<double> > indices;
    std::vector<double> ri;

    for(auto& g : gaps_)
    {
        auto mat = g->medium();

        ri.clear();
        for(auto w : wvl){
            double rndx = mat->rindex(w);
            ri.push_back(rndx);
        }
        indices.push_back(ri);
    }
    ri = indices[indices.size()-1];
    indices.push_back(ri);

    return indices;
}

double SequentialModel::central_wavelength() const
{
    return optical_model_->optical_spec()->spectral_region()->reference_wvl();
}

double SequentialModel::central_rndx(int i)
{
    int central_wvl = optical_model_->optical_spec()->spectral_region()->reference_index();

    if(i >= 0){
        return rndx_[i][central_wvl];
    }
    else{
        int index = rndx_.size() + i;
        return rndx_[index][central_wvl];
    }

}


void SequentialModel::set_cur_surface(int s)
{
    cur_surface_ = s;
}

int SequentialModel::set_stop()
{
    stop_surface_ = cur_surface_;
    return stop_surface_;
}

void SequentialModel::set_stop(int i)
{
    stop_surface_ = i;
}

int SequentialModel::stop_surface() const
{
    return stop_surface_;
}

void SequentialModel::insert(std::shared_ptr<Surface> s, std::shared_ptr<Gap> g)
{
    int num_ifcs = interfaces_.size();
    if(num_ifcs > 2)
    {
        if(stop_surface_ > cur_surface_ /*&& stop_surface_ < num_ifcs-2*/ )
        {
            stop_surface_ += 1;
        }
    }
    cur_surface_ += 1;

    int surf = cur_surface_;

    auto ifcs_itr = interfaces_.begin();
    std::advance(ifcs_itr, surf);
    interfaces_.insert(ifcs_itr,s);

    if(g)
    {
        auto gap_itr = gaps_.begin();
        std::advance(gap_itr,surf);
        gaps_.insert(gap_itr, g);
    }
    else{
        g = gaps_[surf];
    }


    Tfrm tfrm;
    tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
    tfrm.transfer = Eigen::Vector3d::Zero(3);

    auto gbl_tfrm_itr = gbl_tfrms_.begin();
    std::advance(gbl_tfrm_itr, surf);

    auto lcl_tfrm_itr = lcl_tfrms_.begin();
    std::advance(lcl_tfrm_itr, surf);
    lcl_tfrms_.insert(lcl_tfrm_itr, tfrm);

    double new_z_dir;
    if(surf > 1){
        new_z_dir = z_dirs_[surf-1];
    }
    else{
        new_z_dir = 1.0;
    }
    auto z_dir_itr = z_dirs_.begin();
    std::advance(z_dir_itr, surf);
    z_dirs_.insert(z_dir_itr,new_z_dir);

    int num_wvl = optical_model_->optical_spec()->spectral_region()->wvl_count();
    std::vector<double> wvls;
    std::vector<double> rindex;
    for(int i = 0; i < num_wvl; i++)
    {
        double w = optical_model_->optical_spec()->spectral_region()->wavelength(i);
        wvls.push_back(w);
        rindex.push_back(g->medium()->rindex(w));
    }

    auto rndx_itr = rndx_.begin();
    std::advance(rndx_itr, surf);
    rndx_.insert(rndx_itr,rindex);

    if(s->interact_mode() == "reflect"){
        // update_reflections(start=surf)
    }
}


void SequentialModel::insert(int i)
{
    cur_surface_ = i;

    auto s = std::make_shared<Surface>();
    s->profile()->set_cv(0.0);
    auto g = std::make_shared<Gap>(0.0, std::make_shared<Air>());

    this->insert(s, g);
}

void SequentialModel::remove(int i)
{
    if( i < (int)gaps_.size()){
        auto srf_itr = interfaces_.begin();
        interfaces_.erase(srf_itr + i);

        auto gap_itr = gaps_.begin();
        gaps_.erase(gap_itr + i);

        // update stop surface
        if( i < stop_surface_ ) {
            stop_surface_ -= 1;
        }
    }

}

void SequentialModel::add_surface(double radius, double thickness, std::string medium_display_name)
{
    auto s = std::make_shared<Surface>();

    if(radius != 0.0)
    {
        s->profile()->set_cv(1.0/radius);
    }
    else
    {
        s->profile()->set_cv(0.0);
    }

    std::shared_ptr<Gap> g;

    if(medium_display_name == "" || medium_display_name == "AIR")
    {
        g = std::make_shared<Gap>(thickness, std::make_shared<Air>());
    }
    else{
        /*
        std::vector<std::string> g_c = FileIO::split(medium_display_name, '_');
        std::string glass_name = g_c[0];
        std::string catalog_name = g_c[1];
        std::shared_ptr<Glass> mat = optical_model_->database()->glass_catalog(catalog_name)->glass(glass_name);
        */
        std::shared_ptr<Medium> mat = optical_model_->database()->find(medium_display_name);
        if(mat){
            g = std::make_shared<Gap>(thickness, mat);
        }else{
            g = std::make_shared<Gap>(thickness, std::make_shared<Air>());
        }
    }

    //double wvl = optical_model_->optical_spec()->spectral_region()->reference_wvl();
    //double rndx = mat->rindex(wvl);

    this->insert(s, g);
}


void SequentialModel::add_surface(double radius, double thickness, std::shared_ptr<Medium> med)
{
    auto s = std::make_shared<Surface>();

    if(radius != 0.0)
    {
        s->profile()->set_cv(1.0/radius);
    }
    else
    {
        s->profile()->set_cv(0.0);
    }

    auto g = std::make_shared<Gap>(thickness, med);

    this->insert(s, g);
}


void SequentialModel::add_surface(std::unique_ptr<SurfaceProfile> prf, double thickness, std::string medium_display_name)
{
    auto s = std::make_shared<Surface>();

    s->set_profile(std::move(prf));

    std::shared_ptr<Gap> g;
    if(medium_display_name == "" || medium_display_name == "AIR")
    {
        g = std::make_shared<Gap>(thickness, std::make_shared<Air>());
    }
    else{
        std::shared_ptr<Medium> mat = optical_model_->database()->find(medium_display_name);
        if(mat){ //found
            g = std::make_shared<Gap>(thickness, mat);
        }else{   //not found
            g = std::make_shared<Gap>(thickness, std::make_shared<Air>());
        }
    }

    this->insert(s, g);
}


void SequentialModel::set_semi_diameters()
{
    int num_fields = optical_model_->optical_spec()->field_of_view()->field_count();
    //int num_wvls = optical_model_->optical_spec()->spectral_region()->wvl_count();
    int num_srfs = (int)interfaces_.size();

    std::vector<Eigen::Vector2d> pupils;
    pupils.reserve(4);
    pupils.push_back(Eigen::Vector2d({0.0, 1.0}));
    pupils.push_back(Eigen::Vector2d({0.0, -1.0}));
    pupils.push_back(Eigen::Vector2d({1.0, 0.0}));
    pupils.push_back(Eigen::Vector2d({-1.0, 0.0}));

    double wvl = optical_model_->optical_spec()->spectral_region()->reference_wvl();

    Ray r2, r3, r4, r5;
    RayAtSurface ray_at_srf;
    std::vector<double> max_semi_diameters(num_srfs, 0.0);

    for(int fi = 0; fi < num_fields; fi++)
    {
        auto fld = optical_model_->optical_spec()->field_of_view()->field(fi);

        // trace marginal rays
        r2 = Trace::trace_base(*optical_model_, Eigen::Vector2d({ 0.0,  1.0}), *fld, wvl);
        r3 = Trace::trace_base(*optical_model_, Eigen::Vector2d({ 0.0, -1.0}), *fld, wvl);
        r4 = Trace::trace_base(*optical_model_, Eigen::Vector2d({ 1.0,  0.0}), *fld, wvl);
        r5 = Trace::trace_base(*optical_model_, Eigen::Vector2d({-1.0,  0.0}), *fld, wvl);

        // check all surfaces
        Eigen::Vector3d pt;
        for(int si = 0; si < num_srfs; si++)
        {
            pt = r2.at(si).intersect_pt;
            double sd2 = sqrt(pt(0)*pt(0) + pt(1)*pt(1));

            pt = r3.at(si).intersect_pt;
            double sd3 = sqrt(pt(0)*pt(0) + pt(1)*pt(1));

            pt = r4.at(si).intersect_pt;
            double sd4 = sqrt(pt(0)*pt(0) + pt(1)*pt(1));

            pt = r5.at(si).intersect_pt;
            double sd5 = sqrt(pt(0)*pt(0) + pt(1)*pt(1));

            std::vector<double> sd_vec({sd2,sd3,sd4,sd5});
            double max_sd = *std::max_element(sd_vec.begin(), sd_vec.end());

            if(max_semi_diameters[si] < max_sd){
                max_semi_diameters[si] = max_sd;
            }

        }

    }

    // update
    for(int si = 0; si < num_srfs; si++){
        interfaces_[si]->set_semi_diameter(max_semi_diameters[si]);
    }

}


void SequentialModel::print(std::ostringstream& oss)
{
    const int idx_w = 4;
    const int val_w = 16;
    const int prec  = 4;

    // header labels
    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "Radius";
    oss << std::setw(val_w) << std::right << "Thickness";
    oss << std::setw(val_w) << std::right << "Medium";
    oss << std::setw(val_w) << std::right << "Index";
    oss << std::setw(val_w) << std::right << "SemiDiam";
    oss << std::setw(val_w) << std::right << "Aperture";
    oss << std::setw(val_w) << std::right << "Max.Ap";
    oss << std::setw(val_w) << std::right << "Mode";
    oss << std::setw(val_w) << std::right << "Z-Dir";
    oss << std::setw(val_w) << std::right << "LclTfrm(X)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Y)";
    oss << std::setw(val_w) << std::right << "LclTfrm(Z)";
    oss << std::setw(val_w) << std::right << "GblTfrm(X)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Y)";
    oss << std::setw(val_w) << std::right << "GblTfrm(Z)";
    oss << std::endl;

    double ref_wvl = optical_model_->optical_spec()->spectral_region()->reference_wvl();
    Path p = path(ref_wvl);

    for(int i = 0; i < (int)p.size(); i++)
    {
        double c = p[i].srf->profile()->cv();
        double r = 1.0/c;

        if(p[i].gap)
        {
            double thi           = p[i].gap->thi();
            double rind          = p[i].gap->medium()->rindex(ref_wvl);
            std::string med_name = p[i].gap->medium()->display_name();
            std::string imode    = p[i].srf->interact_mode();
            double sd            = p[i].srf->semi_diameter();
            double max_ap        = p[i].srf->max_aperture();
            double z_dir         = p[i].z_dir;

            std::string aperture_type = "None";
            if(p[i].srf->clear_aperture()){
                aperture_type = p[i].srf->clear_aperture()->shape_name();
            }

            if(i == stop_surface_){
                oss << std::setw(idx_w) << std::right << (std::to_string(i) + "*");
            }else{
                oss << std::setw(idx_w) << std::right << i;
            }

            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << r;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << thi;
            oss << std::setw(val_w) << std::right << std::fixed << med_name;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << rind;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << sd;
            oss << std::setw(val_w) << std::right << std::fixed << aperture_type;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << max_ap;
            oss << std::setw(val_w) << std::right << std::fixed << imode;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << z_dir;

            // local transforms
            Eigen::Vector3d lcl_t = p[i].srf->local_transform().transfer;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(0);
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(1);
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << lcl_t(2);

            // global transform
            Eigen::Vector3d gbl_t = p[i].srf->global_transform().transfer;
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(0);
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(1);
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << gbl_t(2);

            oss << std::endl;
        }
    }

    oss << std::ends;

}


std::vector<Tfrm> SequentialModel::compute_global_coords(int glo)
{
    std::vector<Tfrm> tfrms;

    Eigen::Matrix3d r = Eigen::Matrix3d::Identity(3,3);
    Eigen::Vector3d t = Eigen::Vector3d::Zero(3);

    Tfrm prev;
    prev.rotation = r;
    prev.transfer = t;

    tfrms.push_back(prev);

    Path path;
    PathComponent path_comp;

    int go;

    if(glo > 0){
        /* iterate in reverse over the segments before the
         * global reference surface
         */
        go = glo;

        // make path
        int ifcs_itr = glo;
        int gaps_itr = glo - 1;

        for(int i = glo; i >= 0; i--)
        {
            if(ifcs_itr >= 0){
                path_comp.srf = interfaces_[ifcs_itr].get();
            }else{
                path_comp.srf = nullptr;
            }
            ifcs_itr--;


            if(gaps_itr >= 0){
                path_comp.gap = gaps_[gaps_itr].get();
            }else{
                path_comp.gap = nullptr;
            }
            gaps_itr--;

            path.push_back(path_comp);
        }


        auto path_itr = path.begin();
        int path_itr_cnt = 0;
        PathComponent after = *path_itr;
        PathComponent before;

        // loop of remaining surfaces in path
        while(true)
        {
            ++path_itr;
            ++path_itr_cnt;
            before = *path_itr;
            go--;
            if(path_itr_cnt == (int)path.size()) break;

            double zdist = after.gap->thi();
            Tfrm r_t = reverse_transform(*before.srf, zdist, *after.srf);
            Eigen::Matrix3d r = r_t.rotation;
            Eigen::Vector3d t = r_t.transfer;

            t = prev.rotation*t + prev.transfer;
            r = prev.rotation*r;

            prev.rotation = r;
            prev.transfer = t;

            tfrms.push_back(prev);
            after = before;

        } // end while

        std::reverse(tfrms.begin(), tfrms.end());
    } // end if

    path.clear();

    // make path
    int ifcs_size = interfaces_.size();
    int gaps_size = gaps_.size();

    for(int i = glo; i < std::max(ifcs_size, gaps_size); i++){
        if(i < ifcs_size){
            path_comp.srf = interfaces_[i].get();
        }else{
            path_comp.srf = nullptr;
        }
        if(i < gaps_size){
            path_comp.gap = gaps_[i].get();
        }else{
            path_comp.gap = nullptr;
        }

        path.push_back(path_comp);
    }

    auto path_itr = path.begin();
    int path_itr_cnt = 0;
    PathComponent before = *path_itr;
    PathComponent after;

    prev.rotation = Eigen::Matrix3d::Identity(3,3);
    prev.transfer = Eigen::Vector3d::Zero(3);

    go = glo;

    // loop forward over the remaining surfaces in path
    while(true)
    {
        path_itr++;
        path_itr_cnt++;
        after = *path_itr;

        go++;
        if(path_itr_cnt == (int)path.size()) break;
        double zdist = before.gap->thi();

        Tfrm r_t = forward_transform(*before.srf, zdist, *after.srf);
        Eigen::Matrix3d r = r_t.rotation;
        Eigen::Vector3d t = r_t.transfer;

        t = prev.rotation*t + prev.transfer;
        r = prev.rotation*r;

        prev.rotation = r;
        prev.transfer = t;

        tfrms.push_back(prev);

        after.srf->set_global_transform(prev);

        before = after;

    }

    for(int i = 0; i < (int)tfrms.size();i++){
        interfaces_[i]->set_global_transform(tfrms[i]);
    }
    return tfrms;
}

std::vector<Tfrm> SequentialModel::compute_local_transforms()
{
    std::vector<Tfrm> tfrms;

    Path path;
    PathComponent path_comp;
    int ifcs_size = interfaces_.size();
    int gaps_size = gaps_.size();

    for(int i = 0; i < std::max(ifcs_size, gaps_size); i++){
        if(i < ifcs_size){
            path_comp.srf = interfaces_[i].get();
        }else{
            path_comp.srf = nullptr;
        }
        if(i < gaps_size){
            path_comp.gap = gaps_[i].get();
        }else{
            path_comp.gap = nullptr;
        }

        path.push_back(path_comp);
    }

    auto path_itr = path.begin();
    int path_indx_cnt = 0;
    PathComponent before = *path_itr;

    while(true)
    {
        path_itr++;
        path_indx_cnt++;
        PathComponent after = *path_itr;

        double zdist = before.gap->thi();

        auto r_t = forward_transform(*before.srf, zdist, *after.srf);
        Eigen::Matrix3d r = r_t.rotation;
        Eigen::Vector3d t = r_t.transfer;

        Eigen::Matrix3d rt = r.transpose();

        Tfrm tfrm;
        tfrm.rotation = rt;
        tfrm.transfer = t;
        tfrms.push_back(tfrm);

        before.srf->set_local_transform(tfrm);

        before = after;

        //if(path_itr == path.end())
        if(path_indx_cnt == (int)path.size()-1) // last
        {
            //Tfrm tfrm;
            tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
            tfrm.transfer = Eigen::Vector3d::Zero(3);
            tfrms.push_back(tfrm);

            break;
        }
    }


    return tfrms;
}

Tfrm SequentialModel::forward_transform(const Surface& s1, double zdist, const Surface& s2)
{
    //calculate origin of s2 wrt to s1
    Eigen::Vector3d t_orig({0.0, 0.0, zdist});
    Eigen::Matrix3d r_after_s1; // None
    Eigen::Matrix3d r_before_s2; // None

    /*
     * if s1.decenter:
        # get transformation info after s1
        r_after_s1, t_after_s1 = s1.decenter.tform_after_surf()
        t_orig += t_after_s1

    if s2.decenter:
        # get transformation info before s2
        r_before_s2, t_before_s2 = s2.decenter.tform_before_surf()
        t_orig += t_before_s2
        */
    Eigen::Matrix3d r_cascade = Eigen::Matrix3d::Identity(3,3);

    /*
     * if r_after_s1 is not None:
        # rotate the origin of s2 around s1 "after" transformation
        t_orig = r_after_s1.dot(t_orig)
        r_cascade = r_after_s1
        if r_before_s2 is not None:
            r_cascade = r_after_s1.dot(r_before_s2)
    elif r_before_s2 is not None:
        r_cascade = r_before_s2
        */
    Tfrm ret;
    ret.rotation = r_cascade;
    ret.transfer = t_orig;

    return ret;
}

Tfrm SequentialModel::reverse_transform(const Surface& s1, double zdist, const Surface& s2)
{
    Eigen::Vector3d t_orig({0.0, 0.0, zdist});
    Eigen::Matrix3d r_after_s1; // None
    Eigen::Matrix3d r_before_s2; // None

    /*
    if(s1->decenter()){
        //get transformation info after s1
        r_after_s1, t_after_s1 = s1.decenter.tform_after_surf()
        t_orig += t_after_s1
    }

    if(s2->decenter()){
        // get transformation info before s2
        r_before_s2, t_before_s2 = s2.decenter.tform_before_surf()
        t_orig += t_before_s2
    }

    */

    // going in reverse direction so negate translation
    t_orig = -t_orig;

    Eigen::Matrix3d r_cascade = Eigen::Matrix3d::Identity(3,3);

    /*
     * if r_before_s2 is not None:
        # rotate the origin of s1 around s2 "before" transformation
        r_cascade = r_before_s2.transpose()
        t_orig = r_cascade.dot(t_orig)
        if r_after_s1 is not None:
            r_cascade = r_cascade.dot(r_after_s1.transpose())
    elif r_after_s1 is not None:
        r_cascade = r_after_s1.transpose()
        */
    Tfrm ret;
    ret.rotation = r_cascade;
    ret.transfer = t_orig;

    return ret;

}

double SequentialModel::length(int s1, int s2) const
{
    double oal = 0.0;

    for(int i = s1; i < s2-1; i++)
    {
        oal += gaps_[i]->thi();
    }

    return oal;
}


void SequentialModel::compute_vignetting()
{
    //int num_wvls = optical_model_->optical_spec()->spectral_region()->wvl_count();
    int num_flds = optical_model_->optical_spec()->field_of_view()->field_count();

    optical_model_->optical_spec()->update_model();

    for(int fi = 1; fi < num_flds; fi++) {

        Field* fld = optical_model_->optical_spec()->field_of_view()->field(fi);

        double max_vuy = 0.0;
        double max_vly = 0.0;
        double max_vux = 0.0;
        double max_vlx = 0.0;


        double wvl = optical_model_->optical_spec()->spectral_region()->reference_wvl();

        /*
        double vuy = compute_vuy(*fld, wvl);
        double vly = compute_vly(*fld, wvl);
        double vux = compute_vux(*fld, wvl);
        double vlx = compute_vlx(*fld, wvl);
        */
        double vuy = compute_vignetting_factor(*fld, wvl, 2);
        double vly = compute_vignetting_factor(*fld, wvl, 3);
        double vux = compute_vignetting_factor(*fld, wvl, 4);
        double vlx = compute_vignetting_factor(*fld, wvl, 5);

        if(max_vuy < vuy){
            max_vuy = vuy;
        }
        if(max_vly < vly){
            max_vly = vly;
        }
        if(max_vux < vux){
            max_vux = vux;
        }
        if(max_vlx < vlx){
            max_vlx = vlx;
        }


        fld->set_vuy(max_vuy);
        fld->set_vly(max_vly);
        fld->set_vux(max_vux);
        fld->set_vlx(max_vlx);
    }
}

double SequentialModel::compute_vuy(Field& fld, double wvl)
{
    std::cout << "compute_vuy" << std::endl;

    const double eps = 1.0e-3;
    double orig_vuy = fld.vuy();

    Eigen::Vector2d pupil;
    pupil(0) = 0.0;
    pupil(1) = 1.0;

    Ray ray;

    double a = 0.0;
    double b = 1.0; // equal to chief ray
    double m;

    int loop_cnt = 0;

    while(true){

        m = (a+b)/2.0;
        fld.set_vuy(m);

        ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
        if(ray.status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }

        std::cout << loop_cnt << ": (a,b)= " << a << ", " << b << std::endl;;
        loop_cnt ++;
        if( (abs(a-b) < eps) || (loop_cnt > 50)){
            break;
        }
    }

    fld.set_vuy(orig_vuy);

    return b;
}

double SequentialModel::compute_vly(Field& fld, double wvl)
{
    const double eps = 1.0e-3;
    double orig_vly = fld.vly();

    Eigen::Vector2d pupil;
    pupil(0) = 0.0;
    pupil(1) = -1.0;

    Ray ray;

    double a = 0.0;
    double b = 1.0;
    double m;

    int loop_cnt = 0;

    while(true){

        m = (a+b)/2.0;
        fld.set_vly(m);

        ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
        if(ray.status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }

        loop_cnt ++;
        if( abs(a-b) < eps || loop_cnt > 50){
            break;
        }
    }

    fld.set_vly(orig_vly);

    return b;
}


double SequentialModel::compute_vux(Field& fld, double wvl)
{
    const double eps = 1.0e-3;
    double orig_vux = fld.vux();

    Eigen::Vector2d pupil;
    pupil(0) = 1.0;
    pupil(1) = 0.0;

    Ray ray;

    double a = 0.0;
    double b = 1.0;
    double m;

    int loop_cnt = 0;

    while( true ){

        m = (a+b)/2.0;
        fld.set_vux(m);
        ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
        if(ray.status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }

        loop_cnt ++;
        if(abs(a-b) < eps || loop_cnt > 50){
            break;
        }
    }

    fld.set_vux(orig_vux);

    return b;
}


double SequentialModel::compute_vlx(Field& fld, double wvl)
{

    const double eps = 1.0e-3;
    double orig_vlx = fld.vlx();

    Eigen::Vector2d pupil;
    pupil(0) = -1.0;
    pupil(1) = 0.0;

    Ray ray;

    double a = 0.0;
    double b = 1.0;
    double m;

    int loop_cnt = 0;

    while(true){

        m = (a+b)/2.0;
        fld.set_vlx(m);
        ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
        if(ray.status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }


        loop_cnt ++;
        if(abs(a-b) < eps  || loop_cnt > 50){
            break;
        }
    }

    fld.set_vlx(orig_vlx);

    return FileIO::round_n(b, 4);
}


double SequentialModel::compute_vignetting_factor(Field& fld, double wvl, int pupil_ray_index)
{
    const double eps = 1.0e-5;

    double orig_vig;
    Eigen::Vector2d pupil;

    /* Get original value */
    switch (pupil_ray_index) {
    case 2: // tangential upper marginal
        pupil(0) = 0.0;
        pupil(1) = 1.0;
        orig_vig = fld.vuy();
        break;
    case 3: // tangential lower marginal
        pupil(0) = 0.0;
        pupil(1) = -1.0;
        orig_vig = fld.vly();
        break;
    case 4: // sagittal upper marginal
        pupil(0) = 1.0;
        pupil(1) = 0.0;
        orig_vig = fld.vux();
        break;
    case 5: // sagittal lower marginal
        pupil(0) = -1.0;
        pupil(1) = 0.0;
        orig_vig = fld.vlx();
        break;
    }


    /* check the existence of vignetting */
    switch (pupil_ray_index) {
    case 2: // tangential upper marginal
        fld.set_vuy(0.0);
        break;
    case 3: // tangential lower marginal
        fld.set_vly(0.0);
        break;
    case 4: // sagittal upper marginal
        fld.set_vux(0.0);
        break;
    case 5: // sagittal lower marginal
        fld.set_vlx(0.0);
        break;
    }

    Ray ray;
    ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
    if(ray.status() == RayStatus::PassThrough){
        return 0.0; // no vignetting
    }



    /* search edge */
    double a = 0.0;
    double b = 1.0;
    double m;
    int loop_cnt = 0;

    while(true){

        m = (a+b)/2.0;

        switch (pupil_ray_index) {
        case 2: // tangential upper marginal
            fld.set_vuy(m);
            break;
        case 3: // tangential lower marginal
            fld.set_vly(m);
            break;
        case 4: // sagittal upper marginal
            fld.set_vux(m);
            break;
        case 5: // sagittal lower marginal
            fld.set_vlx(m);
            break;
        }

        ray = Trace::trace_base(*optical_model_, pupil, fld, wvl);
        if(ray.status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }

        loop_cnt ++;
        if(abs(a-b) < eps  || loop_cnt > 50){
            break;
        }
    }

    /* restore original vignetting factor */
    switch (pupil_ray_index) {
    case 2: // tangential upper marginal
        fld.set_vuy(orig_vig);
        break;
    case 3: // tangential lower marginal
        fld.set_vly(orig_vig);
        break;
    case 4: // sagittal upper marginal
        fld.set_vux(orig_vig);
        break;
    case 5: // sagittal lower marginal
        fld.set_vlx(orig_vig);
        break;
    }

    //return FileIO::ceil_n(b, 4);
    return b;
}
