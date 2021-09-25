#include "single_ray_trace_dialog.h"
#include "ui_single_ray_trace_dialog.h"

#include <QDebug>
#include <iostream>
#include <sstream>


SingleRayTraceDialog::SingleRayTraceDialog(OpticalSystem *sys, TextViewDock *parent) :
    AnalysisSettingDialog(parent),
    ui(new Ui::SingleRayTraceDialog),
    m_parentDock(parent),
    m_opticalSystem(sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Single Ray Trace Setting");

    // radio buttons
    QObject::connect(ui->traceTypeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(showStackedTab(int)));
    ui->stackedWidget->setCurrentIndex(0);
    ui->pupilRayRadioButton->setChecked(true);

    // set validator to line edits
    ui->objectXEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->objectYEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->objectTanXEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->objectTanYEdit->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->pupilXEdit->setValidator(new QDoubleValidator(-1.0, 1.0, 2, this));
    ui->pupilYEdit->setValidator(new QDoubleValidator(-1.0, 1.0, 2, this));

    // default value
    ui->objectXEdit->setText(QString::number(0.0));
    ui->objectYEdit->setText(QString::number(0.0));
    ui->objectTanXEdit->setText(QString::number(0.0));
    ui->objectTanYEdit->setText(QString::number(0.0));
    ui->pupilXEdit->setText(QString::number(0.0));
    ui->pupilYEdit->setText(QString::number(0.0));

    // field select combo
    const int num_flds = m_opticalSystem->optical_spec()->field_of_view()->field_count();
    for(int i = 0; i < num_flds; i++){
        QString field_item = "F" + QString::number(i+1); //F1,F2...
        ui->fieldCombo->addItem(field_item);
    }

    //wvl combo
    const int num_wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(m_opticalSystem->optical_spec()->spectral_region()->wvl(i)->value());
        ui->wvlForPupilCombo->addItem(wvl_item);
        ui->wvlForObjectCombo->addItem(wvl_item);
    }

    // FIXME : It is unclear why the buttonBox of this dialog itself doesn't work, though that of other dialogs work fine.
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

SingleRayTraceDialog::~SingleRayTraceDialog()
{
    m_opticalSystem = nullptr;
    delete ui;
}

void SingleRayTraceDialog::showStackedTab(int i)
{
    ui->stackedWidget->setCurrentIndex(-i-2);
    ui->stackedWidget->show();
}

void SingleRayTraceDialog::updateParentDockContent()
{
    const int traceType = -2 - ui->traceTypeButtonGroup->checkedId();

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

void SingleRayTraceDialog::doPupilRayTrace()
{
    // Get parameters from UI
    double px = ui->pupilXEdit->text().toDouble();
    double py = ui->pupilYEdit->text().toDouble();
    int fi = ui->fieldCombo->currentIndex();
    Field* fld = m_opticalSystem->optical_spec()->field_of_view()->field(fi);
    int wi = ui->wvlForPupilCombo->currentIndex();
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    //Field *fld = opt_sys_->optical_spec()->field_of_view()->field(fi);
    Eigen::Vector2d pupil_crd({px, py});


    // trace
    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    std::shared_ptr<Ray> ray_trace_result = tracer->trace_pupil_ray(pupil_crd, fld, wvl);
    delete tracer;

    // construct output text
    std::ostringstream oss;
    oss << "Real Ray Trace..." <<  std::endl;
    oss << "Pupil Coordinate: " << "(" << pupil_crd(0) << ", " << pupil_crd(1) << ")" << std::endl;
    oss << "Field: " << fi << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result->print(oss);
    oss << std::endl;

    // write to textview dock
    m_parentDock->setStringStreamToText(oss);


}

void SingleRayTraceDialog::doObjectRayTrace()
{
    // Get parameters from UI
    double x = ui->objectXEdit->text().toDouble();
    double y = ui->objectYEdit->text().toDouble();
    double z = m_opticalSystem->optical_assembly()->gap(0)->thi();
    double tanX = ui->objectTanXEdit->text().toDouble();
    double tanY = ui->objectTanYEdit->text().toDouble();
    double L = tanX;
    double M = tanY;
    double N = 1.0;
    int wi = ui->wvlForObjectCombo->currentIndex();

    Eigen::Vector3d p0({x,y,z});
    Eigen::Vector3d dir0({L,M,N});
    double wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl(wi)->value();

    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    auto ray_trace_result = tracer->trace_ray_throughout_path(tracer->overall_sequential_path(wi), p0, dir0);
    delete tracer;

    std::ostringstream oss;
    oss << "Real Ray Trace..." <<  std::endl;
    oss << "Object Space Point     : " << "(" << x << ", " << y << ")" << std::endl;
    oss << "Object Space Direction : " << "(" << L << ", " << M << ", " << N << ")" << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result->print(oss);
    oss << std::endl;

    m_parentDock->setStringStreamToText(oss);
}

