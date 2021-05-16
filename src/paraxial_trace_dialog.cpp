#include "paraxial_trace_dialog.h"
#include "ui_paraxial_trace_dialog.h"

ParaxialTraceDialog::ParaxialTraceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaxialTraceDialog)
{
    ui->setupUi(this);
}

ParaxialTraceDialog::~ParaxialTraceDialog()
{
    delete ui;
}
