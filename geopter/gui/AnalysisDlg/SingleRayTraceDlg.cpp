#include "SingleRayTraceDlg.h"
#include "ui_SingleRayTraceDlg.h"

#include <QDebug>
#include <iostream>
#include <sstream>


SingleRayTraceDlg::SingleRayTraceDlg(OpticalSystem *sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::SingleRayTraceDlg),
    m_parentDock(parent)
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
    const int num_flds = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    for(int i = 0; i < num_flds; i++){
        QString field_item = "F" + QString::number(i);
        ui->fieldCombo->addItem(field_item);
    }

    //wvl combo
    const int num_wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i) + ": " + QString::number(m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(i)->Value());
        ui->wvlForPupilCombo->addItem(wvl_item);
        ui->wvlForObjectCombo->addItem(wvl_item);
    }

    // FIXME : It is unclear why the buttonBox of this dialog itself doesn't work, though that of other dialogs work fine.
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

SingleRayTraceDlg::~SingleRayTraceDlg()
{
    m_opticalSystem = nullptr;
    delete ui;
}

void SingleRayTraceDlg::showStackedTab(int i)
{
    ui->stackedWidget->setCurrentIndex(-i-2);
    ui->stackedWidget->show();
}

void SingleRayTraceDlg::updateParentDockContent()
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

void SingleRayTraceDlg::doPupilRayTrace()
{
    // Get parameters from UI
    double px = ui->pupilXEdit->text().toDouble();
    double py = ui->pupilYEdit->text().toDouble();
    int fi = ui->fieldCombo->currentIndex();
    Field* fld = m_opticalSystem->GetOpticalSpec()->GetFieldSpec()->GetField(fi);
    int wi = ui->wvlForPupilCombo->currentIndex();
    double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();

    //Field *fld = opt_sys_->GetOpticalSpec()->field_of_view()->field(fi);
    Eigen::Vector2d pupil_crd({px, py});


    // trace
    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    SequentialPath seq_path = tracer->CreateSequentialPath(wvl);
    auto ray_trace_result = std::make_shared<Ray>(seq_path.Size());
    tracer->TracePupilRay(ray_trace_result, seq_path, pupil_crd, fld, wvl);
    delete tracer;

    // construct output text
    std::ostringstream oss;
    oss << "Single Ray Trace..." <<  std::endl;
    oss << "Pupil Coordinate: " << "(" << pupil_crd(0) << ", " << pupil_crd(1) << ")" << std::endl;
    oss << "Field: " << (fi+1) << std::endl;
    oss << "Wavelength(nm): " << wvl << std::endl;

    ray_trace_result->Print(oss);
    oss << std::endl;

    // write to textview dock
    m_parentDock->setText(oss);


}

void SingleRayTraceDlg::doObjectRayTrace()
{
    // Get parameters from UI
    double x = ui->objectXEdit->text().toDouble();
    double y = ui->objectYEdit->text().toDouble();
    double z = m_opticalSystem->GetOpticalAssembly()->GetGap(0)->Thickness();
    double tanX = ui->objectTanXEdit->text().toDouble();
    double tanY = ui->objectTanYEdit->text().toDouble();
    double L = tanX;
    double M = tanY;
    double N = 1.0;
    int wi = ui->wvlForObjectCombo->currentIndex();

    Eigen::Vector3d p0({x,y,z});
    Eigen::Vector3d dir0({L,M,N});
    double wvl = m_opticalSystem->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();

    SequentialTrace *tracer = new SequentialTrace(m_opticalSystem);
    SequentialPath seq_path = tracer->CreateSequentialPath(wvl);
    auto ray_trace_result = std::make_shared<Ray>(seq_path.Size());
    tracer->TraceRayThroughoutPath(ray_trace_result, seq_path, p0, dir0);
    delete tracer;

    std::ostringstream oss;
    oss << "Real Ray Trace..." <<  std::endl;
    oss << "Object Space Point     : " << "(" << x << ", " << y << ")" << std::endl;
    oss << "Object Space Direction : " << "(" << L << ", " << M << ", " << N << ")" << std::endl;
    oss << "Wavelength: " << wi << " " << wvl << std::endl;

    ray_trace_result->Print(oss);
    oss << std::endl;

    m_parentDock->setText(oss);
}

