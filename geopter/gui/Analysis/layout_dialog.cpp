#include "layout_dialog.h"
#include "ui_layout_dialog.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

LayoutDialog::LayoutDialog(PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::LayoutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Layout Setting");

    m_parentDock = parent;

    ui->checkDrawRefRay->setChecked(true);
    ui->editNumRays->setValidator(new QIntValidator(1, 20, this));
    ui->editNumRays->setText(QString::number(10));

    //QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

LayoutDialog::~LayoutDialog()
{
    delete ui;
}

/*
void LayoutDialog::onAccept()
{
    accept();
}
*/
void LayoutDialog::getSettings(bool *doDrawRefRays, bool *doDrawFanRays, int *numberOfFanRays)
{
    *doDrawRefRays = ui->checkDrawRefRay->checkState();
    *doDrawFanRays = ui->checkDrawFan->checkState();
    *numberOfFanRays = ui->editNumRays->text().toDouble();
}
