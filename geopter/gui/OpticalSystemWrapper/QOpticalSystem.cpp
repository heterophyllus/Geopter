#include "QOpticalSystem.h"

#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "gui/PythonQtScriptingConsole.h"

#include "paraxial/paraxial_trace.h"

QOpticalSystem::QOpticalSystem() : QObject(nullptr), OpticalSystem()
{
    lde_ = std::make_unique<QLensDataEditor>(this);
}

QOpticalSystem::~QOpticalSystem()
{
    lde_.reset();
}

QLensDataEditor* QOpticalSystem::LensDataEditor() const
{
    return lde_.get();
}

QString QOpticalSystem::GetTitle() const
{
    return QString().fromStdString(title_);
}

QString QOpticalSystem::GetNote() const
{
    return QString().fromStdString(note_);
}

double QOpticalSystem::EntrancePupilDiameter() const
{
    return fod_->enp_radius*2.0;
}

double QOpticalSystem::EntrancePupilDistance() const
{
    return fod_->enp_dist;
}

double QOpticalSystem::ExitPupilDiameter() const
{
    return fod_->exp_radius*2.0;
}

double QOpticalSystem::ExitPupilDistance() const
{
    return fod_->exp_dist;
}

int QOpticalSystem::NumberOfFields() const
{
    return opt_spec_->field_of_view()->field_count();
}

int QOpticalSystem::NumberOfWavelengths() const
{
    return opt_spec_->spectral_region()->number_of_wavelengths();
}

int QOpticalSystem::NumberOfSurfaces() const
{
    return opt_assembly_->surface_count();
}

double QOpticalSystem::FocalLength(int start, int end) const
{
    if(start < end){
        PythonQt::self()->pythonStdErr("Invalid surface range");
        return qQNaN();
    }else if( !CheckSurfaceIndex(start)) {
        PythonQt::self()->pythonStdErr("Surface out of range:" + QString::number(start));
        return qQNaN();
    }else if(!CheckSurfaceIndex(end)) {
        PythonQt::self()->pythonStdErr("Surface out of range:" + QString::number(end));
        return qQNaN();
    }

    const int ref_wi = opt_spec_->spectral_region()->reference_index();

    ParaxialTrace *tracer = new ParaxialTrace(this);
    Eigen::Matrix2d M = tracer->system_matrix(start, end, ref_wi);
    delete tracer;

    double f = -1.0/M(1,0);
    return f;
}

double QOpticalSystem::FocalLength() const
{
    const int s1 = 1;
    const int s2 = opt_assembly_->image_index() - 1;

    return this->FocalLength(s1, s2);
}

void QOpticalSystem::SetTitle(QString title)
{
    title_ = title.toStdString();
}

void QOpticalSystem::SetNote(QString note)
{
    note_ = note.toStdString();
}

void QOpticalSystem::PythonStdOut(const QString& text) const
{
    PythonQt::self()->pythonStdErr(text);
}

bool QOpticalSystem::CheckSurfaceIndex(int si) const
{
    if(si < 0){
        return false;
    }
    else if(si <= opt_assembly_->image_index()){
        return true;
    }
    else{
        return false;
    }
}
