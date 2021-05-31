#include <iomanip>

#include "paraxial_trace_dialog.h"
#include "ui_paraxial_trace_dialog.h"

#include "text_view_dock.h"

ParaxialTraceDialog::ParaxialTraceDialog(OpticalModel* opt_model, TextViewDock *parent) :
    QDialog(parent),
    ui(new Ui::ParaxialTraceDialog),
    parentDock_(parent),
    opt_model_(opt_model)
{
    ui->setupUi(this);
    this->setWindowTitle("Paraxial Ray Trace Setting");

    //wvl combo
    const int num_wvl = opt_model_->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(opt_model_->optical_spec()->spectral_region()->wavelength(i));
        ui->comboWvl->addItem(wvl_item);
    }

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

ParaxialTraceDialog::~ParaxialTraceDialog()
{
    delete ui;
}

void ParaxialTraceDialog::onAccept()
{
    int wi = ui->comboWvl->currentIndex();
    double wvl = opt_model_->optical_spec()->spectral_region()->wvl(wi)->value();

    ParaxialRay ax_ray, pr_ray;
    opt_model_->paraxial_model()->compute_paraxial_ray(ax_ray, pr_ray, wvl);


    const int idx_w  = 4;
    const int val_w  = 10;
    const int prec   = 4;
    std::ostringstream oss;

    oss << "Paraxial Trace" << std::endl;
    oss << std::endl;
    oss << "Wavelength: " << wvl << std::endl;
    oss << std::endl;

    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "HMY";
    oss << std::setw(val_w) << std::right << "UMY";
    oss << std::setw(val_w) << std::right << "N*IMY";
    oss << std::setw(val_w) << std::right << "HCY";
    oss << std::setw(val_w) << std::right << "UCY";
    oss << std::setw(val_w) << std::right << "N*ICY";
    oss << std::endl;

    assert( ax_ray.size() == pr_ray.size() );
    for(int i = 0; i < ax_ray.size(); i++) {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).slp;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ax_ray.at(i).n * ax_ray.at(i).aoi;

        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).slp;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << pr_ray.at(i).n * pr_ray.at(i).aoi;

        oss << std::endl;
    }



    // write to textview dock
    parentDock_->setStringStreamToText(oss);

    accept();
}
