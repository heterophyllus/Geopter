#include "spec_setting_dialog.h"
#include "ui_spec_setting_dialog.h"

#include "float_delegate.h"

#include <QtWidgets>
#include <QDebug>

SpecSettingDialog::SpecSettingDialog(std::shared_ptr<OpticalSystem> opt_sys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpecSettingDialog),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);

    this->setWindowTitle("Optical Specs Setting");

    setUpPupilTab();
    setUpWvlTab();
    setUpFieldTab();

    syncUiWithModel();

    QObject::connect( ui->pushButton_AddWvl,    SIGNAL(clicked()), this, SLOT(addWvl()) );
    QObject::connect( ui->pushButton_RemoveWvl, SIGNAL(clicked()), this, SLOT(removeWvl()) );
    QObject::connect( ui->tableWidget_Wvls,     SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onWvlCellDoubleClicked(int,int)) );

    QObject::connect( ui->pushButton_AddField,    SIGNAL(clicked()), this, SLOT(addField()) );
    QObject::connect( ui->pushButton_RemoveField, SIGNAL(clicked()), this, SLOT(removeField()) );
    QObject::connect( ui->tableWidget_Fields,     SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onFieldCellDoubleClicked(int,int)) );

    QObject::connect( ui->pushButton_SetVig,    SIGNAL(clicked()), this, SLOT(buttonSetVignettingClicked()) );

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

    ui->tabWidget->setCurrentIndex(0);
}

SpecSettingDialog::~SpecSettingDialog()
{
    opt_sys_ = nullptr;

    delete ui;
}

void SpecSettingDialog::onAccept()
{
    syncModelWithUi();
    opt_sys_->update_model();
    accept();
}

void SpecSettingDialog::setOpticalSystem(std::shared_ptr<OpticalSystem> opt_model)
{
    opt_sys_.reset();
    opt_sys_ = opt_model;
}

void SpecSettingDialog::syncModelWithUi()
{
    QTableWidget *table;

    // pupil spec
    double pupil_value = ui->lineEdit_PupilValue->text().toDouble();
    int pupil_type = ui->comboBox_PupilType->currentIndex();

    opt_sys_->optical_spec()->pupil_spec()->set_pupil_type(pupil_type);
    opt_sys_->optical_spec()->pupil_spec()->set_value(pupil_value);

    // wvl spec
    table = ui->tableWidget_Wvls;
    int num_wvls = table->rowCount();
    opt_sys_->optical_spec()->spectral_region()->clear();
    for(int wi = 0; wi < num_wvls; wi++) {
        double val   = table->item(wi, WvlTab::WvlValue)->text().toDouble();
        double wt    = table->item(wi, WvlTab::WvlWeight)->text().toDouble();
        QColor color = table->item(wi, WvlTab::WvlColor)->background().color();

        opt_sys_->optical_spec()->spectral_region()->add(val, wt, QColorToRgb(color));
    }

    int refWvl = ui->comboBox_Ref_Wvl->currentIndex();
    opt_sys_->optical_spec()->spectral_region()->set_reference_index(refWvl);


    // field spec
    opt_sys_->optical_spec()->field_of_view()->clear();

    int field_type = ui->comboBox_FieldType->currentIndex();
    opt_sys_->optical_spec()->field_of_view()->set_field_type(field_type);

    table = ui->tableWidget_Fields;
    int num_flds = table->rowCount();


    for(int fi = 0; fi < num_flds; fi++) {
        double x  = table->item(fi, FieldTab::FldX)->text().toDouble();
        double y  = table->item(fi, FieldTab::FldY)->text().toDouble();
        double wt = table->item(fi, FieldTab::FldWeight)->text().toDouble();
        double vlx = table->item(fi, FieldTab::FldVLX)->text().toDouble();
        double vux = table->item(fi, FieldTab::FldVUX)->text().toDouble();
        double vly = table->item(fi, FieldTab::FldVLY)->text().toDouble();
        double vuy = table->item(fi, FieldTab::FldVUY)->text().toDouble();
        QColor color = table->item(fi, FieldTab::FldColor)->background().color();

        opt_sys_->optical_spec()->field_of_view()->add(x,y,wt,QColorToRgb(color));

        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vlx(vlx);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vux(vux);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vuy(vuy);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vly(vly);

    }

}


void SpecSettingDialog::syncUiWithModel()
{
    QTableWidget *table;

    // Pupil
    setPupilData(*opt_sys_->optical_spec()->pupil_spec());

    // Wvl
    table = ui->tableWidget_Wvls;
    table->setRowCount(0);
    int num_wvl = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    for(int wi = 0; wi < num_wvl; wi++) {
        addWvl();
        setWvlData(wi, *opt_sys_->optical_spec()->spectral_region()->wvl(wi));
    }
    int refWvl = opt_sys_->optical_spec()->spectral_region()->reference_index();
    ui->comboBox_Ref_Wvl->setCurrentIndex(refWvl);

    // Field
    table = ui->tableWidget_Fields;
    table->setRowCount(0);
    int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    for(int fi = 0; fi < num_flds; fi++) {
        addField();
        setFldData(fi, *opt_sys_->optical_spec()->field_of_view()->field(fi));
    }
}


/******************************************************************************************************
 * Pupil Tab
 * ****************************************************************************************************/
void SpecSettingDialog::setUpPupilTab()
{
    QStringList pupilTypes({ "Entrance Pupil Diameter",
                             "F Number",
                             "Numerical Aperture (Image Space)",
                             "Numerical Aperture (Object Space)" });

    ui->comboBox_PupilType->addItems(pupilTypes);

    ui->lineEdit_PupilValue->setValidator(new QDoubleValidator(0, 100, 10, this));
    ui->lineEdit_PupilValue->setText(QString::number(10));
}

void SpecSettingDialog::setPupilData(const PupilSpec& pupil)
{
    ui->comboBox_PupilType->setCurrentIndex(pupil.pupil_type());
    ui->lineEdit_PupilValue->setText(QString::number(pupil.value()));
}



/******************************************************************************************************
 * Wavelength Tab
 * ****************************************************************************************************/
void SpecSettingDialog::setUpWvlTab()
{
    QTableWidget *table = ui->tableWidget_Wvls;
    table->setItemDelegate(new FloatDelegate(4));

    QStringList hHeaderLabels({"Wavelength", "Weight", "Render Color"});
    table->setColumnCount(hHeaderLabels.size());
    table->setHorizontalHeaderLabels(hHeaderLabels);

}

void SpecSettingDialog::addWvl()
{
    QTableWidget *table = ui->tableWidget_Wvls;
    int rowCount = table->rowCount();

    table->insertRow(rowCount);
    int currentRow = table->rowCount()-1;

    setValueToCell(table, currentRow, WvlTab::WvlValue, 587.6);
    setValueToCell(table, currentRow, WvlTab::WvlWeight, 1.0);
    setColorToCell(table, currentRow, WvlTab::WvlColor, Qt::black);

    setCellUneditable(table, currentRow, WvlTab::WvlColor);

    updateReferenceWvlCombo();
}

void SpecSettingDialog::removeWvl()
{
    QTableWidget *table = ui->tableWidget_Wvls;

    if(table->rowCount() == 1){
        return;
    }

    int currentRow = table->currentRow();
    table->removeRow(currentRow);

    updateReferenceWvlCombo();
}



void SpecSettingDialog::updateReferenceWvlCombo()
{
    int num_wvls = ui->tableWidget_Wvls->rowCount();

    ui->comboBox_Ref_Wvl->clear();

    for(int i = 0; i < num_wvls; i++){
        QString text = "W" + QString::number(i+1);
        ui->comboBox_Ref_Wvl->addItem(text);
    }

    ui->comboBox_Ref_Wvl->setCurrentIndex(opt_sys_->optical_spec()->spectral_region()->reference_index());
}


void SpecSettingDialog::setWvlData(int row, const Wvl& wvl)
{
    QTableWidget *table = ui->tableWidget_Wvls;
    if(row >= table->rowCount()) {
        return;
    }

    // set data
    setValueToCell(table, row, WvlTab::WvlValue, wvl.value());
    setValueToCell(table, row, WvlTab::WvlWeight, wvl.weight());
    setColorToCell(table, row, WvlTab::WvlColor, rgbToQColor(wvl.render_color()));
}

void SpecSettingDialog::onWvlCellDoubleClicked(int row, int col)
{
    if(col == WvlTab::WvlValue) {
        selectSpectralLine(row, col);
    }else if(col == WvlTab::WvlColor) {
        selectWvlColor(row, col);
    }
}

void SpecSettingDialog::selectWvlColor(int row, int col)
{
    QTableWidget *table = ui->tableWidget_Wvls;

    if (col == WvlTab::WvlColor) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            setColorToCell(table, row, col, color);
        }
    }
}

void SpecSettingDialog::selectSpectralLine(int row, int col)
{
    QStringList items;
    items << "t" << "s" << "r" << "C" << "C_" << "D" << "d" << "e" << "F" << "F_" << "g" << "h" << "i";

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Input Wavelength"), tr("Select spectral line or input value directly."), items, 0,true, &ok);

    if(ok){

        if(items.contains(item)){ //select item from the list
            double val = SpectralLine::wavelength(item.toStdString());
            setValueToCell(ui->tableWidget_Wvls, row, col,val);
        }else{
            bool isDouble;
            double val = item.toDouble(&isDouble);
            if(isDouble){ // directly input value
                setValueToCell(ui->tableWidget_Wvls, row, col,val);
            }else{
                QMessageBox::warning(nullptr,"Error","Invalid input.");
            }
        }

    }

}

/******************************************************************************************************
 * Field Tab
 * ****************************************************************************************************/
void SpecSettingDialog::setUpFieldTab()
{
    // filed types
    QStringList fieldTypes({"Object Angle",
                            "Object Height",
                            "Paraxial Image Height"});
    ui->comboBox_FieldType->addItems(fieldTypes);

    // field values
    QTableWidget *table = ui->tableWidget_Fields;
    QStringList hHeaderLabels({"X", "Y", "Weight", "Color", "VUY", "VLY", "VUX", "VLX"});

    table->setColumnCount(hHeaderLabels.size());
    table->setHorizontalHeaderLabels(hHeaderLabels);
    table->setRowCount(0);

    table->setItemDelegate(new FloatDelegate(4));
}

void SpecSettingDialog::addField()
{
    QTableWidget *table = ui->tableWidget_Fields;
    int rowCount = table->rowCount();

    table->insertRow(rowCount);
    int currentRow = table->rowCount()-1;

    setValueToCell(table, currentRow,FieldTab::FldX, 0.0);
    setValueToCell(table, currentRow,FieldTab::FldY, 0.0);
    setValueToCell(table, currentRow,FieldTab::FldWeight, 1.0);
    setValueToCell(table, currentRow,FieldTab::FldVLX, 0.0);
    setValueToCell(table, currentRow,FieldTab::FldVUX, 0.0);
    setValueToCell(table, currentRow,FieldTab::FldVLY, 0.0);
    setValueToCell(table, currentRow,FieldTab::FldVUY, 0.0);

    // color
    setColorToCell(table, currentRow, FieldTab::FldColor, Qt::black);
    setCellUneditable(table, currentRow, FieldTab::FldColor);
}

void SpecSettingDialog::removeField()
{
    QTableWidget *table = ui->tableWidget_Fields;

    if(table->rowCount() == 1){
        return;
    }

    int currentRow = table->currentRow();
    table->removeRow(currentRow);
}

void::SpecSettingDialog::onFieldCellDoubleClicked(int row, int col)
{
    if(col == FieldTab::FldColor){
        selectFldColor(row, col);
    }
}

void SpecSettingDialog::selectFldColor(int row, int col)
{
    QTableWidget *table = ui->tableWidget_Fields;

    if (col == FieldTab::FldColor) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            setColorToCell(table, row, col, color);
        }
    }
}


void SpecSettingDialog::setFldData(int row, const Field& fld)
{
    QTableWidget *table = ui->tableWidget_Fields;
    if(row >= table->rowCount()) {
        return;
    }

    setValueToCell(table, row, FieldTab::FldX,      fld.x());
    setValueToCell(table, row, FieldTab::FldY,      fld.y());
    setValueToCell(table, row, FieldTab::FldWeight, fld.weight());
    setColorToCell(table, row, FieldTab::FldColor,  rgbToQColor(fld.render_color()));
    setValueToCell(table, row, FieldTab::FldVUX,    fld.vux());
    setValueToCell(table, row, FieldTab::FldVLX,    fld.vlx());
    setValueToCell(table, row, FieldTab::FldVUY,    fld.vuy());
    setValueToCell(table, row, FieldTab::FldVLY,    fld.vly());
}


void SpecSettingDialog::buttonSetVignettingClicked()
{
    QTableWidget *table = ui->tableWidget_Fields;

    int num_fld = opt_sys_->optical_spec()->field_of_view()->field_count();
    std::vector<double> vig_factors;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_.get());

    for(int fi = 1; fi < num_fld; fi++){
        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);
        vig_factors = tracer->compute_vignetting_factors(*fld);
        qDebug() << ("F" + QString::number(fi) + ":") << vig_factors[0] << vig_factors[1] << vig_factors[2] << vig_factors[3];

        double vuy = vig_factors[0];
        double vly = vig_factors[1];
        double vux = vig_factors[2];
        double vlx = vig_factors[3];

        /*
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vuy(vuy);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vly(vly);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vux(vux);
        opt_sys_->optical_spec()->field_of_view()->field(fi)->set_vlx(vlx);
        */
        setValueToCell(table, fi, FieldTab::FldVUX,    vux);
        setValueToCell(table, fi, FieldTab::FldVLX,    vlx);
        setValueToCell(table, fi, FieldTab::FldVUY,    vuy);
        setValueToCell(table, fi, FieldTab::FldVLY,    vly);

    }
    delete tracer;

    //this->syncModelWithUi();
}

/******************************************************************************************************
 * Other functions
 * ****************************************************************************************************/
void SpecSettingDialog::setValueToCell(QTableWidget* table, int row, int col, double val)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }

    item->setTextAlignment(Qt::AlignRight);

    item->setText(QString::number(val));

}


void SpecSettingDialog::setValueToCell(QTableWidget* table, int row, int col, QString str)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }
    item->setTextAlignment(Qt::AlignRight);
    item->setText(str);
}

void SpecSettingDialog::setColorToCell(QTableWidget* table, int row, int col, QColor color)
{
    if (table) {
        QTableWidgetItem *item = table->item(row, col);
        if (!item) {
            item = new QTableWidgetItem;
            table->setItem(row, col, item);
        }
        table->item(row, col)->setBackground(QBrush(color));
    }

}

void SpecSettingDialog::setCellUneditable(QTableWidget* table, int row, int col)
{
    QTableWidgetItem* item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col,item);
    }
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
}

QColor SpecSettingDialog::rgbToQColor(Rgb rgbColor)
{
    int r = (int)(255.0*rgbColor.r);
    int g = (int)(255.0*rgbColor.g);
    int b = (int)(255.0*rgbColor.b);
    //int a = (int)(255.0*rgbColor.a);
    int a = 255;
    return QColor(r,g,b,a);
}

Rgb SpecSettingDialog::QColorToRgb(QColor color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}
