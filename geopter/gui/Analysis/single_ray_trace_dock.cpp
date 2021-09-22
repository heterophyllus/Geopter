#include "single_ray_trace_dock.h"
#include "Analysis/single_ray_trace_dialog.h"

SingleRayTraceDock::SingleRayTraceDock(QString label, OpticalSystem* sys,  QWidget *parent) :
    TextViewDock(label, parent),
    m_opticalSystem(sys)
{
    m_settingDlgPtr = std::make_unique<SingleRayTraceDialog>(m_opticalSystem, this);
}

SingleRayTraceDock::~SingleRayTraceDock()
{

}

void SingleRayTraceDock::updateText()
{
    m_opticalSystem->update_model();

    int traceType = dynamic_cast<SingleRayTraceDialog*>(m_settingDlgPtr.get())->getTraceType();
    switch (traceType) {
    case 0: // pupil
        doPupilRayTrace();
        break;
    case 1: // object
        doObjectRayTrace();
        break;
    default:
        qDebug() << "Invalid Trace Type";
    }
}

void SingleRayTraceDock::doPupilRayTrace()
{
    double px, py;
    int fi, wi;
    dynamic_cast<SingleRayTraceDialog*>(m_settingDlgPtr.get())->getSettingsForPupilRayTrace(&px, &py, &fi, &wi);

    Eigen::Vector2d pupil_crd({px, py});
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    // trace
    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    Ray ray_trace_result = tracer->trace_pupil_ray(pupil_crd, fi, wi);
    delete tracer;

    // construct output text
    std::ostringstream oss;
    oss << "Single Ray Trace..." <<  std::endl;
    oss << "Pupil Coordinate: " << "(" << pupil_crd(0) << ", " << pupil_crd(1) << ")" << std::endl;
    oss << "Field: " << (fi+1) << std::endl;
    oss << "Wavelength: " << wvl << std::endl;

    ray_trace_result.print(oss);
    oss << std::endl;

    // write to textview dock
    setStringStreamToText(oss);
}

void SingleRayTraceDock::doObjectRayTrace()
{
    double x, y, z, tanX, tanY;
    int wi;

    dynamic_cast<SingleRayTraceDialog*>(m_settingDlgPtr.get())->getSettingsForObjectRayTrace(&x, &y, &tanX, &tanY, &wi);

    z = m_opticalSystem->optical_assembly()->gap(0)->thi();

    double L = tanX;
    double M = tanY;
    double N = 1.0;

    Eigen::Vector3d p0({x,y,z});
    Eigen::Vector3d dir0({L,M,N});
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    Ray ray_trace_result = tracer->trace_ray_throughout_path(tracer->overall_sequential_path(wi), p0, dir0);
    delete tracer;

    std::ostringstream oss;
    oss << "Single Ray Trace..." <<  std::endl;
    oss << "Object Space Point     : " << "(" << x << ", " << y << ")" << std::endl;
    oss << "Object Space Direction : " << "(" << L << ", " << M << ", " << N << ")" << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result.print(oss);
    oss << std::endl;

    setStringStreamToText(oss);
}
