
#include "MainWindow.h"
#include "QApplication"

QString MainWindow::app_dir()
{
    return QApplication::applicationDirPath();
}

void MainWindow::add_field(double x, double y, double wt, QColor color, double vux, double vlx, double vuy, double vly)
{
    opt_sys_->optical_spec()->field_of_view()->add(x, y, wt, QColorToRgb(color), vuy, vly, vux, vlx);
    syncUiWithSystem();
}

void MainWindow::add_wavelength(double wl, double wt, QColor color)
{
    opt_sys_->optical_spec()->spectral_region()->add(wl, wt, QColorToRgb(color));
    syncUiWithSystem();
}

void MainWindow::chromatic_focus_shift()
{
    showChromaticFocusShift();
}

void MainWindow::delete_surface(int s)
{
    if( !check_surface_range(s) ) return;

    opt_sys_->optical_assembly()->remove(s);
    syncUiWithSystem();
}

void MainWindow::draw2d()
{
    showLayout();
}

void MainWindow::field_curvature()
{
    showFieldCurvature();
}

QString MainWindow::glass(int s)
{
    return QString().fromStdString( opt_sys_->optical_assembly()->gap(s)->material()->name() );
}

double MainWindow::indx()
{
    return opt_sys_->optical_spec()->spectral_region()->reference_wvl();
}

void MainWindow::insert_surface(int s)
{
    opt_sys_->optical_assembly()->insert_surface(s);
    syncUiWithSystem();
}

void MainWindow::load_lens(QString filename)
{
    opt_sys_->load_file(filename.toStdString());
    opt_sys_->update_model();
    m_systemEditorDock->setOpticalSystem(opt_sys_);
    //m_systemEditorDock->syncUiWithSystem();
    //PythonQt::self()->pythonStdOut("Optical system loaded");
}


void MainWindow::longitudinal()
{
    showLongitudinal();
}

void MainWindow::new_lens()
{
    opt_sys_->initialize();
    //opt_sys_->update_model();

    m_systemEditorDock->setOpticalSystem(opt_sys_);
    //m_systemEditorDock->syncUiWithSystem();
}

int MainWindow::nfld()
{
    return opt_sys_->optical_spec()->field_of_view()->field_count();
}

int MainWindow::nsur()
{
    return opt_sys_->optical_assembly()->surface_count();
}

int MainWindow::nwav()
{
    return opt_sys_->optical_spec()->spectral_region()->wvl_count();
}

void MainWindow::paraxial_trace()
{
    const int wi = opt_sys_->optical_spec()->spectral_region()->reference_index();
    auto ax_ray = opt_sys_->paraxial_data()->axial_ray(wi);
    auto pr_ray = opt_sys_->paraxial_data()->principle_ray(wi);

    std::ostringstream oss;
    ax_ray->print(oss);
    pr_ray->print(oss);

    std::cout << oss.str() << std::endl;
}

void MainWindow::prescription()
{
    showPrescription();
}

int MainWindow::pwav()
{
    return opt_sys_->optical_spec()->spectral_region()->reference_index();
}

void MainWindow::raytrace(double px, double py, int fi, int wi)
{
    if(wi < 0){
        wi = opt_sys_->optical_spec()->spectral_region()->reference_index();
    }

    Field *fld = opt_sys_->optical_spec()->field_of_view()->field(fi);
    double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();

    SequentialTrace *tracer = new SequentialTrace(opt_sys_.get());
    auto ray = tracer->trace_pupil_ray(Eigen::Vector2d({px, py}), fld, wvl);

    std::ostringstream oss;
    ray->print(oss);

    PythonQt::self()->pythonStdOut(QString().fromStdString(oss.str()));
}

void MainWindow::save_lens(QString filename)
{
    opt_sys_->save_to_file(filename.toStdString());
    PythonQt::self()->pythonStdOut("saved to " + filename);
}

void MainWindow::set_field_type(int type)
{
    opt_sys_->optical_spec()->field_of_view()->set_field_type(type);
    syncUiWithSystem();
}

void MainWindow::set_note(QString note)
{
    opt_sys_->set_note(note.toStdString());
    syncUiWithSystem();
}

void MainWindow::set_pupil_type(int type)
{
    opt_sys_->optical_spec()->pupil_spec()->set_pupil_type(type);
    syncUiWithSystem();
}

void MainWindow::set_pupil_value(double value)
{
    opt_sys_->optical_spec()->pupil_spec()->set_value(value);
    syncUiWithSystem();
}


void MainWindow::set_surface_glas(int s, QString glassname)
{
    if( !check_surface_range(s) ) return;

    geopter::Material* glass = opt_sys_->material_lib()->find(glassname.toStdString()).get();

    if(glass){
        opt_sys_->optical_assembly()->gap(s)->set_material(glass);
        syncUiWithSystem();
    }else{
        PythonQt::self()->pythonStdErr("glass not found");
    }
}

void MainWindow::set_surface_radi(int s, double r)
{
    if( !check_surface_range(s) ) return;

    opt_sys_->optical_assembly()->surface(s)->profile()->set_radius(r);
    this->syncUiWithSystem();
}

void MainWindow::set_surface_thic(int s, double t)
{
    if( !check_surface_range(s) ) return;

    opt_sys_->optical_assembly()->gap(s)->set_thi(t);

    syncUiWithSystem();
}

void MainWindow::set_title(QString title)
{
    opt_sys_->set_title(title.toStdString());
    syncUiWithSystem();
}

void MainWindow::spot_diagram()
{
    showSpotDiagram();
}

int MainWindow::stop_surf()
{
    return opt_sys_->optical_assembly()->stop_index();
}

double MainWindow::radi(int s)
{
    if( !check_surface_range(s) ) return NAN;

    return opt_sys_->optical_assembly()->surface(s)->profile()->radius();
}

double MainWindow::thic(int s)
{
    if( !check_surface_range(s) ) return NAN;

    return opt_sys_->optical_assembly()->gap(s)->thi();
}

void MainWindow::transverse()
{
    showTransverseRayFan();
}

void MainWindow::update_ui()
{
    //m_systemEditorDock->syncUiWithSystem();
}


bool MainWindow::check_surface_range(int s)
{
    if( 0 < s && s < nsur()){
        return true;
    }else{
        PythonQt::self()->pythonStdErr("surface index out of range");
        return false;
    }
}

Rgb MainWindow::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}
