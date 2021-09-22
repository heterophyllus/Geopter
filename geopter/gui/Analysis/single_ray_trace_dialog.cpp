#include "single_ray_trace_dialog.h"
#include "ui_single_ray_trace_dialog.h"

#include <QDebug>
#include <iostream>
#include <sstream>

#include "text_view_dock.h"
#include "optical.h"
using namespace geopter;

SingleRayTraceDialog::SingleRayTraceDialog(OpticalSystem *sys, TextViewDock *parent) :
    QDialog(parent),
    ui(new Ui::SingleRayTraceDialog),
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

int SingleRayTraceDialog::getTraceType() const
{
    return  ( -2 - ui->traceTypeButtonGroup->checkedId() );
}

void SingleRayTraceDialog::getSettingsForPupilRayTrace(double *px, double *py, int *fi, int *wi) const
{
    *px = ui->pupilXEdit->text().toDouble();
    *py = ui->pupilYEdit->text().toDouble();
    *fi = ui->fieldCombo->currentIndex();
    *wi = ui->wvlForPupilCombo->currentIndex();
}

void SingleRayTraceDialog::getSettingsForObjectRayTrace(double *x, double *y, double *tanX, double *tanY, int *wi) const
{
    *x = ui->objectXEdit->text().toDouble();
    *y = ui->objectYEdit->text().toDouble();
    *tanX = ui->objectTanXEdit->text().toDouble();
    *tanY = ui->objectTanYEdit->text().toDouble();
    *wi = ui->wvlForObjectCombo->currentIndex();
}
