#include "SpecEditorDlg.h"
#include "ui_SpecEditorDlg.h"
#include "SystemEditor/SystemDataConstant.h"
#include "FieldTableView.h"
#include "FieldTableModel.h"
#include "WavelengthTableView.h"
#include "WavelengthTableModel.h"
#include <QMenu>
#include <QDebug>

SpecEditorDlg::SpecEditorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpecEditorDlg)
{
    ui->setupUi(this);

    //pupil
    QStringList pupilTypeList({"Entrance Pupil Diameter", "F-Number", "NA(Image Space)", "NA(Object Space"});
    ui->pupilTypeCombo->addItems(pupilTypeList);
    ui->pupilValueEdit->setValidator(new QDoubleValidator(this));

    //field
    QStringList fieldTypeList({"Object Angle","Object Height","Paraxial Image Height"});
    ui->fieldTypeCombo->addItems(fieldTypeList);
    ui->fieldTable->init();

    // in modal dialog, context menu doesn't work, so we implement buttons instead
    QObject::connect(ui->insertFieldButton, SIGNAL(clicked()), ui->fieldTable, SLOT(insertRow()));
    QObject::connect(ui->removeFieldButton, SIGNAL(clicked()), ui->fieldTable, SLOT(removeRow()));
    QObject::connect(ui->addFieldButton,    SIGNAL(clicked()), ui->fieldTable, SLOT(addRow()));

    // wavelength
    QObject::connect(ui->insertWavelengthButton, SIGNAL(clicked()), ui->wavelengthTable, SLOT(insertRow()));
    QObject::connect(ui->removeWavelengthButton, SIGNAL(clicked()), ui->wavelengthTable, SLOT(removeRow()));
    QObject::connect(ui->addWavelengthButton,    SIGNAL(clicked()), ui->wavelengthTable, SLOT(addRow()));
    QObject::connect(ui->wavelengthTable,        SIGNAL(setupCompleted()), this, SLOT(setupReferenceWavelengthCombo()));
    QObject::connect(ui->wavelengthTable,        SIGNAL(valueEdited()), this, SLOT(setupReferenceWavelengthCombo()));

}

SpecEditorDlg::~SpecEditorDlg()
{
    delete ui;
}

void SpecEditorDlg::catchValueEdited()
{
    qDebug() << "Value Edited!";
}

void SpecEditorDlg::loadData(const std::shared_ptr<OpticalSystem> optsys)
{
    // pupil
    int pupilType = optsys->optical_spec()->pupil_spec()->pupil_type();
    ui->pupilTypeCombo->setCurrentIndex(pupilType);

    double pupilValue = optsys->optical_spec()->pupil_spec()->value();
    ui->pupilValueEdit->setText(QString::number(pupilValue));

    // fields
    int fieldType = optsys->optical_spec()->field_of_view()->field_type();
    ui->fieldTypeCombo->setCurrentIndex(fieldType);
    dynamic_cast<FieldTableModel*>(ui->fieldTable->model())->setData(optsys);

    // wavelengths
    int refWvlIdx = optsys->optical_spec()->spectral_region()->reference_index();
    dynamic_cast<WavelengthTableModel*>(ui->wavelengthTable->model())->setData(optsys);
    setupReferenceWavelengthCombo(refWvlIdx);

    // title/note
    std::string title = optsys->title();
    ui->titleEdit->setText(QString().fromStdString(title));

    std::string note = optsys->note();
    ui->noteEdit->setText(QString().fromStdString(note));

    // environment
    double temperature = Environment::temperature();
    ui->temperatureEdit->setText(QString::number(temperature));
}

void SpecEditorDlg::applyData(std::shared_ptr<OpticalSystem> optsys)
{
    //pupil
    int pupilType = ui->pupilTypeCombo->currentIndex();
    optsys->optical_spec()->pupil_spec()->set_pupil_type(pupilType);

    double pupilValue = ui->pupilValueEdit->text().toDouble();
    optsys->optical_spec()->pupil_spec()->set_value(pupilValue);

    // fields
    int fieldType = ui->fieldTypeCombo->currentIndex();
    optsys->optical_spec()->field_of_view()->set_field_type(fieldType);
    dynamic_cast<FieldTableModel*>(ui->fieldTable->model())->applyData(optsys);

    // wavelength
    dynamic_cast<WavelengthTableModel*>(ui->wavelengthTable->model())->applyData(optsys);
    int ref = ui->referenceWavelengthCombo->currentIndex();
    optsys->optical_spec()->spectral_region()->set_reference_index(ref);

    //title/note
    QString title = ui->titleEdit->text();
    optsys->set_title(title.toStdString());

    QString note = ui->noteEdit->toPlainText();
    optsys->set_note(note.toStdString());

    // environment
    double t = ui->temperatureEdit->text().toDouble();
    Environment::set_temperature(t);

    optsys->update_model();
}


void SpecEditorDlg::setupReferenceWavelengthCombo(int current)
{
    if(current < 0) {
        current = ui->referenceWavelengthCombo->currentIndex();
    }
    int wvlCount = dynamic_cast<WavelengthTableModel*>(ui->wavelengthTable->model())->rowCount();
    QStringList wavelengthList;
    for(int wi = 0; wi < wvlCount; wi++){
        // W0: 630.00
        // W1: 588.00
        // W2: 475.00
        // ...
        int row = wi;
        int col = static_cast<int>(WavelengthTableColumn::Value);
        QModelIndex index = dynamic_cast<WavelengthTableModel*>(ui->wavelengthTable->model())->index(row, col);
        double val = dynamic_cast<WavelengthTableModel*>(ui->wavelengthTable->model())->data(index).toDouble();
        wavelengthList << "W" + QString::number(wi) + ": " + QString::number(val);
    }

    ui->referenceWavelengthCombo->clear();
    ui->referenceWavelengthCombo->addItems(wavelengthList);
    ui->referenceWavelengthCombo->setCurrentIndex(current);
}
