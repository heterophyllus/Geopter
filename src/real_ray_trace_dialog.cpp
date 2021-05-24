#include "real_ray_trace_dialog.h"
#include "ui_real_ray_trace_dialog.h"

#include <QDebug>
#include <iostream>
#include <sstream>

#include "text_view_dock.h"
#include "optical_core.h"
using namespace geopter;

RealRayTraceDialog::RealRayTraceDialog(OpticalModel* opt_model, TextViewDock *parent) :
    QDialog(parent),
    ui(new Ui::RealRayTraceDialog),
    parentDock(parent),
    opm(opt_model)
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
    const int num_flds = opm->optical_spec()->field_of_view()->field_count();
    for(int i = 0; i < num_flds; i++){
        QString field_item = "F" + QString::number(i+1); //F1,F2...
        ui->comboBox_Field->addItem(field_item);
    }

    //wvl combo
    const int num_wvl = opm->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(opm->optical_spec()->spectral_region()->wavelength(i));
        ui->comboBox_Wvl_Pupil->addItem(wvl_item);
        ui->comboBox_Wvl_Obj->addItem(wvl_item);
    }

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

RealRayTraceDialog::~RealRayTraceDialog()
{
    delete ui;
}

void RealRayTraceDialog::showStackedTab(int i)
{
    ui->stackedWidget->setCurrentIndex(-i-2);
    ui->stackedWidget->show();
}

void RealRayTraceDialog::setOpticalModel(OpticalModel *opt_model)
{
    opm = opt_model;
}

void RealRayTraceDialog::onAccept()
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

void RealRayTraceDialog::doPupilRayTrace()
{
    // Get parameters from UI
    double px = ui->lineEdit_Pupil_X->text().toDouble();
    double py = ui->lineEdit_Pupil_Y->text().toDouble();
    int fi = ui->comboBox_Field->currentIndex();
    int wi = ui->comboBox_Wvl_Pupil->currentIndex();

    Field *fld = opm->optical_spec()->field_of_view()->field(fi);
    Eigen::Vector2d pupil_crd({px, py});
    double wvl = opm->optical_spec()->spectral_region()->wvl(wi)->value();

    // trace
    Ray ray_trace_result = Trace::trace_pupil_ray(*opm, pupil_crd, *fld, wvl);

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

void RealRayTraceDialog::doObjectRayTrace()
{
    // Get parameters from UI
    double x = ui->lineEdit_Obj_X->text().toDouble();
    double y = ui->lineEdit_Obj_Y->text().toDouble();
    double z = opm->seq_model()->gap(0)->thi();
    double tanX = ui->lineEdit_Obj_TanX->text().toDouble();
    double tanY = ui->lineEdit_Obj_TanY->text().toDouble();
    double L = tanX;
    double M = tanY;
    double N = 1.0;
    int wi = ui->comboBox_Wvl_Obj->currentIndex();

    Eigen::Vector3d p0({x,y,z});
    Eigen::Vector3d dir0({L,M,N});
    double wvl = opm->optical_spec()->spectral_region()->wvl(wi)->value();

    Ray ray_trace_result = Trace::trace_ray_from_object(*opm->seq_model(), p0, dir0, wvl);

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

