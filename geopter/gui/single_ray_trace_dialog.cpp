#include "single_ray_trace_dialog.h"
#include "ui_single_ray_trace_dialog.h"

#include <QDebug>
#include <iostream>
#include <sstream>

#include "text_view_dock.h"
#include "optical.h"
using namespace geopter;

SingleRayTraceDialog::SingleRayTraceDialog(OpticalSystem* opt_sys, TextViewDock *parent) :
    QDialog(parent),
    ui(new Ui::SingleRayTraceDialog),
    parentDock(parent),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Single Ray Trace Setting");

    // radio buttons
    QObject::connect(ui->buttonGroup_TraceType, SIGNAL(buttonClicked(int)), this, SLOT(showStackedTab(int)));
    ui->stackedWidget->setCurrentIndex(0);
    ui->radioButton_Pupil->setChecked(true);

    // set validator to line edits
    ui->lineEdit_Obj_X->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->lineEdit_Obj_Y->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->lineEdit_Obj_TanX->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->lineEdit_Obj_TanY->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->lineEdit_Pupil_X->setValidator(new QDoubleValidator(-1.0, 1.0, 2, this));
    ui->lineEdit_Pupil_Y->setValidator(new QDoubleValidator(-1.0, 1.0, 2, this));

    // default value
    ui->lineEdit_Obj_X->setText(QString::number(0.0));
    ui->lineEdit_Obj_Y->setText(QString::number(0.0));
    ui->lineEdit_Obj_TanX->setText(QString::number(0.0));
    ui->lineEdit_Obj_TanY->setText(QString::number(0.0));
    ui->lineEdit_Pupil_X->setText(QString::number(0.0));
    ui->lineEdit_Pupil_Y->setText(QString::number(0.0));

    // field select combo
    const int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    for(int i = 0; i < num_flds; i++){
        QString field_item = "F" + QString::number(i+1); //F1,F2...
        ui->comboBox_Field->addItem(field_item);
    }

    //wvl combo
    const int num_wvl = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(opt_sys_->optical_spec()->spectral_region()->wavelength(i));
        ui->comboBox_Wvl_Pupil->addItem(wvl_item);
        ui->comboBox_Wvl_Obj->addItem(wvl_item);
    }

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

SingleRayTraceDialog::~SingleRayTraceDialog()
{
    delete ui;
}

void SingleRayTraceDialog::showStackedTab(int i)
{
    ui->stackedWidget->setCurrentIndex(-i-2);
    ui->stackedWidget->show();
}

void SingleRayTraceDialog::setOpticalSystem(OpticalSystem *opt_sys)
{
    opt_sys_ = opt_sys;
}

void SingleRayTraceDialog::onAccept()
{
    const int traceType = -2 - ui->buttonGroup_TraceType->checkedId();

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

    accept();
}

void SingleRayTraceDialog::doPupilRayTrace()
{
    // Get parameters from UI
    double px = ui->lineEdit_Pupil_X->text().toDouble();
    double py = ui->lineEdit_Pupil_Y->text().toDouble();
    int fi = ui->comboBox_Field->currentIndex();
    int wi = ui->comboBox_Wvl_Pupil->currentIndex();

    //Field *fld = opt_sys_->optical_spec()->field_of_view()->field(fi);
    Eigen::Vector2d pupil_crd({px, py});
    double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();

    // trace
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    Ray ray_trace_result = tracer->trace_pupil_ray(pupil_crd, fi, wi);
    delete tracer;

    // construct output text
    std::ostringstream oss;
    oss << "Real Ray Trace..." <<  std::endl;
    oss << "Pupil Coordinate: " << "(" << pupil_crd(0) << ", " << pupil_crd(1) << ")" << std::endl;
    oss << "Field: " << fi << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result.print(oss);
    oss << std::ends;

    // write to textview dock
    parentDock->setStringStreamToText(oss);

    //std::cout << oss.str() << std::endl;
}

void SingleRayTraceDialog::doObjectRayTrace()
{
    // Get parameters from UI
    double x = ui->lineEdit_Obj_X->text().toDouble();
    double y = ui->lineEdit_Obj_Y->text().toDouble();
    double z = opt_sys_->optical_assembly()->gap(0)->thi();
    double tanX = ui->lineEdit_Obj_TanX->text().toDouble();
    double tanY = ui->lineEdit_Obj_TanY->text().toDouble();
    double L = tanX;
    double M = tanY;
    double N = 1.0;
    int wi = ui->comboBox_Wvl_Obj->currentIndex();

    Eigen::Vector3d p0({x,y,z});
    Eigen::Vector3d dir0({L,M,N});
    double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    Ray ray_trace_result = tracer->trace_ray_throughout_path(tracer->overall_sequential_path(wi), p0, dir0);
    delete tracer;

    std::ostringstream oss;
    oss << "Real Ray Trace..." <<  std::endl;
    oss << "Object Space Point     : " << "(" << x << ", " << y << ")" << std::endl;
    oss << "Object Space Direction : " << "(" << L << ", " << M << ", " << N << ")" << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result.print(oss);
    oss << std::ends;

    parentDock->setStringStreamToText(oss);

    //std::cout << oss.str() << std::endl;
}

