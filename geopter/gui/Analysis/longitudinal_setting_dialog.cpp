#include "longitudinal_setting_dialog.h"
#include "ui_longitudinal_setting_dialog.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LongitudinalSettingDialog::LongitudinalSettingDialog(PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::LongitudinalSettingDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Longitudinal Aberration Setting");

    m_parentDock = parent;

    ui->editLsaScale->setValidator(new QDoubleValidator(0.0, 1.0e+10, 4, this));
    ui->editLsaScale->setText(QString::number(0.1));
    ui->editAstScale->setValidator(new QDoubleValidator(0.0, 1.0e+10, 4, this));
    ui->editAstScale->setText(QString::number(0.1));

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

LongitudinalSettingDialog::~LongitudinalSettingDialog()
{
    delete ui;
}

void LongitudinalSettingDialog::onAccept()
{
    accept();
}

double LongitudinalSettingDialog::getScale()
{
    double lsa_scale = ui->editLsaScale->text().toDouble();
    return lsa_scale;
}
