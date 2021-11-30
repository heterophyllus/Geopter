#include "SpecEditorDlg.h"
#include "ui_SpecEditorDlg.h"
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

    // in modal dialog, context menu doesn't work, so we implement buttons instead
    QObject::connect(ui->insertFieldButton, SIGNAL(clicked()), ui->fieldTable, SLOT(insertField()));
    QObject::connect(ui->removeFieldButton, SIGNAL(clicked()), ui->fieldTable, SLOT(removeField()));
    QObject::connect(ui->addFieldButton,    SIGNAL(clicked()), ui->fieldTable, SLOT(addField()));

    // wavelength
    QObject::connect(ui->insertWavelengthButton, SIGNAL(clicked()), ui->wavelengthTable, SLOT(insertWavelength()));
    QObject::connect(ui->removeWavelengthButton, SIGNAL(clicked()), ui->wavelengthTable, SLOT(removeWavelength()));
    QObject::connect(ui->addWavelengthButton,    SIGNAL(clicked()), ui->wavelengthTable, SLOT(addWavelength()));

}

SpecEditorDlg::~SpecEditorDlg()
{
    delete ui;
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
    ui->fieldTable->importFieldData(optsys);

    // wavelengths
    ui->wavelengthTable->importWavelengthData(optsys);
    int refWvlIdx = optsys->optical_spec()->spectral_region()->reference_index();
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
    ui->fieldTable->applyCurrentData(optsys);

    // wavelength
    ui->wavelengthTable->applyCurrentData(optsys);
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
    int wvlCount = ui->wavelengthTable->rowCount();
    QStringList wavelengthList;
    for(int wi = 0; wi < wvlCount; wi++){
        // W0: 630.00
        // W1: 588.00
        // W2: 475.00
        // ...
        double val = ui->wavelengthTable->item(wi, WavelengthTable::Value)->data(Qt::EditRole).toDouble();
        wavelengthList << "W" + QString::number(wi) + ": " + QString::number(val);
    }

    ui->referenceWavelengthCombo->clear();
    ui->referenceWavelengthCombo->addItems(wavelengthList);
    ui->referenceWavelengthCombo->setCurrentIndex(current);
}
