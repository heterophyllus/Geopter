#include <iomanip>

#include "paraxial_trace_dialog.h"
#include "ui_paraxial_trace_dialog.h"

#include "text_view_dock.h"

ParaxialTraceDialog::ParaxialTraceDialog(OpticalSystem* opt_sys, TextViewDock *parent) :
    QDialog(parent),
    ui(new Ui::ParaxialTraceDialog),
    m_parentDock(parent),
    m_opticalSystem(opt_sys)
{
    ui->setupUi(this);
    this->setWindowTitle("Paraxial Ray Trace Setting");

    //wvl combo
    const int num_wvl = m_opticalSystem->optical_spec()->spectral_region()->wvl_count();
    for(int i = 0; i < num_wvl; i++){
        QString wvl_item = "W" + QString::number(i+1) + ": " + QString::number(m_opticalSystem->optical_spec()->spectral_region()->wvl(i)->value());
        ui->comboWvl->addItem(wvl_item);
    }

}

ParaxialTraceDialog::~ParaxialTraceDialog()
{
    delete ui;
}

void ParaxialTraceDialog::getSettings(int *wi)
{
    *wi = ui->comboWvl->currentIndex();
}
