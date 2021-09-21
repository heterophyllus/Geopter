#include "system_editor_widget.h"
#include "ui_system_editor_widget.h"

#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QTableWidget>

#include "float_delegate.h"
#include "surface_property_dialog.h"

using namespace geopter;

SystemEditorWidget::SystemEditorWidget(std::shared_ptr<OpticalSystem> opt_sys, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemEditorWidget),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);

    initialize();

    //======================
    // Assembly
    ui->assemblyTable->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->assemblyTable->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnAssemblyTableHeader()));

    ui->assemblyTable->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->assemblyTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnAssemblyTableCell()));

    QObject::connect(ui->assemblyTable, SIGNAL(cellChanged(int,int)), this, SLOT(validateCellInputOnAssemblyTable(int,int)));
    QObject::connect(ui->assemblyTable, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(validateCellDoubleClickOnAssemblyTable(QTableWidgetItem*)));

    FloatDelegate *delegate = new FloatDelegate(4, ui->assemblyTable);
    ui->assemblyTable->setItemDelegate(delegate);

    ui->assemblyTable->setEditTriggers(QAbstractItemView::DoubleClicked);


    //======================
    // Spec
    ui->pupilValueEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 8, this));
    QObject::connect( ui->wavelengthTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(validateCellDoubleClickOnWavelengthTable(int,int)) );
    QObject::connect( ui->fieldTable,      SIGNAL(cellDoubleClicked(int, int)), this, SLOT(validateCellDoubleClickOnFieldTable(int,int)) );

    syncUiWithSystem();

    ui->tabWidget->setCurrentIndex(0); // show assembly tab
}

SystemEditorWidget::~SystemEditorWidget()
{
    delete ui;
}

//*********************************************************************************************************************************************
// Common
void SystemEditorWidget::setOpticalSystem(std::shared_ptr<geopter::OpticalSystem> opt_sys)
{
    opt_sys_ = opt_sys;
}

void SystemEditorWidget::initialize()
{
    //=========================
    // Assembly
    //QStringList assemblyTableHorizontalHeaderLabels = {"Label", "Surface Type", "Radius", "Thickness", "Material", "Mode", "SemiDiameter"};
    ui->assemblyTable->clear();
    ui->assemblyTable->setRowCount(1);
    ui->assemblyTable->setColumnCount(assemblyTableColumnCount);
    ui->assemblyTable->setHorizontalHeaderLabels({"Label", "Surface Type", "Radius", "Thickness", "Material", "Mode", "SemiDiameter"});


    //=========================
    // Spec

    // pupil
    ui->pupilTypeCombo->clear();
    ui->pupilTypeCombo->addItems({"Entrance Pupil Diameter", "F-Number", "NA(Image Space)", "NA(Object Space"});
    ui->pupilValueEdit->setText(QString::number(10));

    // wavelength
    ui->wavelengthTable->clear();
    ui->wavelengthTable->setRowCount(1);
    ui->wavelengthTable->setColumnCount(wavelengthTableColumnCount);
    ui->wavelengthTable->setHorizontalHeaderLabels({"Wavelength", "Weight", "Render Color"});
    //updateReferenceWavelengthCombo();

    // field
    ui->fieldTypeCombo->clear();
    ui->fieldTypeCombo->addItems({"Object Angle","Object Height","Paraxial Image Height"});
    ui->fieldTable->clear();
    ui->fieldTable->setRowCount(1);
    ui->fieldTable->setColumnCount(fieldTableColumncount);
    ui->fieldTable->setHorizontalHeaderLabels({"X", "Y", "Weight", "Color", "VUY", "VLY", "VUX", "VLX"});
    ui->fieldTable->setItemDelegate(new FloatDelegate(4));

}

void SystemEditorWidget::syncUiWithSystem()
{
    // =========================================
    // assembly
    int surfaceCount = opt_sys_->optical_assembly()->surface_count();
    ui->assemblyTable->setRowCount(surfaceCount);

    int imageIndex = opt_sys_->optical_assembly()->image_index();
    for(int row = 0; row <= imageIndex; row++) {
        std::string surfaceLabel = opt_sys_->optical_assembly()->surface(row)->label();

        std::string materialName = opt_sys_->optical_assembly()->gap(row)->material()->name();
        std::string surfaceType  = opt_sys_->optical_assembly()->surface(row)->profile()->name();
        std::string interactMode = opt_sys_->optical_assembly()->surface(row)->interact_mode();
        double sd                = opt_sys_->optical_assembly()->surface(row)->semi_diameter();
        double max_ap            = opt_sys_->optical_assembly()->surface(row)->max_aperture();
        std::string apertureType = opt_sys_->optical_assembly()->surface(row)->aperture_shape();

        setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::Label, QString().fromStdString(surfaceLabel));
        setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::Material, QString().fromStdString(materialName));
        setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::Mode, QString().fromStdString(interactMode));
        setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::SurfaceType,QString().fromStdString(surfaceType));
        setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::SemiDiameter, sd);

        //if(row < imageIndex){
            double r   = opt_sys_->optical_assembly()->surface(row)->profile()->radius();
            double thi = opt_sys_->optical_assembly()->gap(row)->thi();

            setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::Radius, r);
            setContentToCell(ui->assemblyTable, row, AssemblyTableColumn::Thickness, thi);
        //}

    }


    // ==========================================
    // spec

    // general
    QString systemTitle = QString().fromStdString(opt_sys_->title());
    QString systemNote = QString().fromStdString(opt_sys_->note());
    ui->titleEdit->setText(systemTitle);
    ui->noteEdit->setText(systemNote);


    // pupil
    int pupilType = opt_sys_->optical_spec()->pupil_spec()->pupil_type();
    ui->pupilTypeCombo->setCurrentIndex(pupilType);

    double pupilValue = opt_sys_->optical_spec()->pupil_spec()->value();
    ui->pupilValueEdit->setText(QString::number(pupilValue));


    // wavelength
    int wvlCount = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    ui->wavelengthTable->setRowCount(wvlCount);

    for (int wi = 0; wi < wvlCount; wi++) {
        double value  = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        double weight = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->weight();
        Rgb    color  = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        setContentToCell(ui->wavelengthTable, wi, WavelengthTableColumn::WavelengthValue, value);
        setContentToCell(ui->wavelengthTable, wi, WavelengthTableColumn::WavelengthWeight, weight);
        setContentToCell(ui->wavelengthTable, wi, WavelengthTableColumn::WavelengthColor, rgbToQColor(color));
    }

    updateReferenceWavelengthCombo();
    int referenceWavelengthIndex = opt_sys_->optical_spec()->spectral_region()->reference_index();
    ui->referenceWavelengthCombo->setCurrentIndex(referenceWavelengthIndex);


    // field
    int fieldType = opt_sys_->optical_spec()->field_of_view()->field_type();
    ui->fieldTypeCombo->setCurrentIndex(fieldType);

    int fieldCount = opt_sys_->optical_spec()->field_of_view()->field_count();
    ui->fieldTable->setRowCount(fieldCount);

    for(int fi = 0; fi < fieldCount; fi++) {
        double x   = opt_sys_->optical_spec()->field_of_view()->field(fi)->x();
        double y   = opt_sys_->optical_spec()->field_of_view()->field(fi)->y();
        double wt  = opt_sys_->optical_spec()->field_of_view()->field(fi)->weight();
        Rgb color  = opt_sys_->optical_spec()->field_of_view()->field(fi)->render_color();
        double vux = opt_sys_->optical_spec()->field_of_view()->field(fi)->vux();
        double vlx = opt_sys_->optical_spec()->field_of_view()->field(fi)->vlx();
        double vuy = opt_sys_->optical_spec()->field_of_view()->field(fi)->vuy();
        double vly = opt_sys_->optical_spec()->field_of_view()->field(fi)->vly();

        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldX, x);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldY, y);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldWeight, wt);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldColor, rgbToQColor(color));
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldVLX, vlx);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldVUX, vux);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldVLY, vly);
        setContentToCell(ui->fieldTable, fi, FieldTableColumn::FieldVUY, vuy);
    }

}

void SystemEditorWidget::syncSystemWithUi()
{
    // =========================================
    // assembly
    opt_sys_->optical_assembly()->clear();
    int surfaceCount = ui->assemblyTable->rowCount();
    for(int si = 0; si < surfaceCount; si++) {
        std::string surfaceLabel = ui->assemblyTable->item(si, AssemblyTableColumn::Label)->text().toStdString();
        double r = ui->assemblyTable->item(si, AssemblyTableColumn::Radius)->text().toDouble();
        double thi = ui->assemblyTable->item(si, AssemblyTableColumn::Thickness)->text().toDouble();
        std::string materialName = ui->assemblyTable->item(si, AssemblyTableColumn::Material)->text().toStdString();

        opt_sys_->add_surface_and_gap(r, thi, materialName);
        opt_sys_->optical_assembly()->surface(opt_sys_->optical_assembly()->surface_count()-1)->set_label(surfaceLabel);
    }



    //===========================================
    // Spec
    opt_sys_->optical_spec()->clear();

    // pupil
    int pupilType = ui->pupilTypeCombo->currentIndex();
    opt_sys_->optical_spec()->pupil_spec()->set_pupil_type(pupilType);

    double pupilValue = ui->pupilValueEdit->text().toDouble();
    opt_sys_->optical_spec()->pupil_spec()->set_value(pupilValue);


    // wavelength
    int wvlCount = ui->wavelengthTable->rowCount();
    for(int wi = 0; wi < wvlCount; wi++) {
        double value = ui->wavelengthTable->item(wi, WavelengthTableColumn::WavelengthValue)->text().toDouble();
        double wt    = ui->wavelengthTable->item(wi, WavelengthTableColumn::WavelengthWeight)->text().toDouble();
        QColor color = ui->wavelengthTable->item(wi, WavelengthTableColumn::WavelengthWeight)->background().color();

        opt_sys_->optical_spec()->spectral_region()->add(value, wt, QColorToRgb(color));
    }

    int referenceWvlIndex = ui->referenceWavelengthCombo->currentIndex();
    opt_sys_->optical_spec()->spectral_region()->set_reference_index(referenceWvlIndex);


    // field
    int fieldType = ui->fieldTypeCombo->currentIndex();
    opt_sys_->optical_spec()->field_of_view()->set_field_type(fieldType);

    int fieldCount = ui->fieldTable->rowCount();
    for(int fi = 0; fi < fieldCount; fi++) {
        double x     = ui->fieldTable->item(fi, FieldTableColumn::FieldX)->text().toDouble();
        double y     = ui->fieldTable->item(fi, FieldTableColumn::FieldY)->text().toDouble();
        double wt    = ui->fieldTable->item(fi, FieldTableColumn::FieldWeight)->text().toDouble();
        double vlx   = ui->fieldTable->item(fi, FieldTableColumn::FieldVLX)->text().toDouble();
        double vux   = ui->fieldTable->item(fi, FieldTableColumn::FieldVUX)->text().toDouble();
        double vly   = ui->fieldTable->item(fi, FieldTableColumn::FieldVLY)->text().toDouble();
        double vuy   = ui->fieldTable->item(fi, FieldTableColumn::FieldVUY)->text().toDouble();
        QColor color = ui->fieldTable->item(fi, FieldTableColumn::FieldColor)->background().color();

        opt_sys_->optical_spec()->field_of_view()->add(x,y,wt,QColorToRgb(color),vuy,vly,vux,vlx);
    }

    opt_sys_->update_model();

}

void SystemEditorWidget::setContentToCell(QTableWidget *table, int row, int col, double value)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }
    item->setTextAlignment(Qt::AlignRight);
    item->setText(QString::number(value));
}

void SystemEditorWidget::setContentToCell(QTableWidget *table, int row, int col, QString str)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }
    item->setTextAlignment(Qt::AlignRight);
    item->setText(str);
}

void SystemEditorWidget::setContentToCell(QTableWidget *table, int row, int col, QColor color)
{
    QTableWidgetItem *item = table->item(row, col);
    if (!item) {
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }
    table->item(row, col)->setBackground(QBrush(color));
}

QColor SystemEditorWidget::rgbToQColor(Rgb rgbColor)
{
    int r = (int)(255.0*rgbColor.r);
    int g = (int)(255.0*rgbColor.g);
    int b = (int)(255.0*rgbColor.b);
    //int a = (int)(255.0*rgbColor.a);
    int a = 255;

    return QColor(r,g,b,a);
}

Rgb SystemEditorWidget::QColorToRgb(QColor color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}


//*********************************************************************************************************************************
// Assembly Tab

void SystemEditorWidget::showContextMenuOnAssemblyTableHeader()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Property");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

    contextMenu.exec(QCursor::pos());
}

void SystemEditorWidget::showContextMenuOnAssemblyTableCell()
{
    QMenu contextMenu;
    contextMenu.clear();

    // actions in context menu
    if(ui->assemblyTable->currentRow() == 0){ // Obj
        QAction *action2 = contextMenu.addAction("Insert after");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertAfterCurrentLineOnAssemblyTable()));
    }
    else if(ui->assemblyTable->currentRow() == ui->assemblyTable->rowCount()-1){ // Img
        QAction *action1 = contextMenu.addAction("Insert before");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertBeforeCurrentLineOnAssemblyTable()));
    }
    else{
        QAction *action1 = contextMenu.addAction("Insert before");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertBeforeCurrentLineOnAssemblyTable()));

        QAction *action2 = contextMenu.addAction("Insert after");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertAfterCurrentLineOnAssemblyTable()));

        QAction *action3 = contextMenu.addAction("Remove");
        QObject::connect(action3, SIGNAL(triggered()), this, SLOT(removeLineFromAssemblyTable()));
    }

    contextMenu.exec(QCursor::pos());
}

void SystemEditorWidget::insertBeforeCurrentLineOnAssemblyTable()
{
    int currentRow = ui->assemblyTable->currentRow();
    insertLineOnAssemblyTable(currentRow);
    updateVerticalHeaderOnAssemblyTable();
}

void SystemEditorWidget::insertAfterCurrentLineOnAssemblyTable()
{
    int currentRow = ui->assemblyTable->currentRow();
    insertLineOnAssemblyTable(currentRow + 1);
    updateVerticalHeaderOnAssemblyTable();
}

void SystemEditorWidget::removeLineFromAssemblyTable()
{
    int currentRow = ui->assemblyTable->currentRow();
    ui->assemblyTable->removeRow(currentRow);
    opt_sys_->optical_assembly()->remove(currentRow);

    updateVerticalHeaderOnAssemblyTable();
}

void SystemEditorWidget::insertLineOnAssemblyTable(int row)
{
    //connectValidateCell(false);

    if(ui->assemblyTable->rowCount() == 0){
        ui->assemblyTable->setRowCount(1);
    }else{
        ui->assemblyTable->insertRow(row);
    }

    // set header item with surface number
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(QString::number(row));
    ui->assemblyTable->setVerticalHeaderItem(row,item);

    // add surface to optical system
    opt_sys_->optical_assembly()->insert_dummy(row);
    opt_sys_->update_model();


    // input cell values
    //syncRowWithModel(row);

    //connectValidateCell(true);
}

void SystemEditorWidget::updateVerticalHeaderOnAssemblyTable()
{
    const int rowCount = ui->assemblyTable->rowCount();

    // surface number
    for(int i = 0; i < rowCount; i++)
    {
        QTableWidgetItem *item = ui->assemblyTable->verticalHeaderItem(i);
        if(!item){
            item = new QTableWidgetItem;
            ui->assemblyTable->setVerticalHeaderItem(i, item);
        }
        ui->assemblyTable->verticalHeaderItem(i)->setText(QString::number(i));
    }

    // OBJ, IMG, STO
    ui->assemblyTable->verticalHeaderItem(0)->setText("OBJ");
    ui->assemblyTable->verticalHeaderItem(rowCount - 1)->setText("IMG");
    if( rowCount > 2 ){
        if(opt_sys_){
            int stopIndex = opt_sys_->optical_assembly()->stop_index();
            ui->assemblyTable->verticalHeaderItem(stopIndex)->setText("STO");
        }
    }
}

void SystemEditorWidget::validateCellDoubleClickOnAssemblyTable(QTableWidgetItem *item)
{
    if(item) {

        int row = item->row();
        int col = item->column();

        switch (col) {
        case AssemblyTableColumn::Label :
            break;
        case AssemblyTableColumn::Radius :
            break;
        case AssemblyTableColumn::Material :
            break;
        case AssemblyTableColumn::Thickness :
            break;
        case AssemblyTableColumn::SurfaceType :
            showSurfacePropertyDlg(row);
            break;
        case AssemblyTableColumn::Mode :
            showSurfacePropertyDlg(row);
            break;
        case AssemblyTableColumn::SemiDiameter :
            showSurfacePropertyDlg(row);
            break;
        default:
            break;
        }

        if(ui->assemblyTable->isPersistentEditorOpen(item)){
            ui->assemblyTable->closePersistentEditor(item);
        }

    }
}

void SystemEditorWidget::validateCellInputOnAssemblyTable(int row, int col)
{
    QTableWidgetItem *item = ui->assemblyTable->item(row, col);
    if(item) {
        return;
    }

    switch (col) {
    case AssemblyTableColumn::Label:
        validateLabelInput(row);
        break;
    case AssemblyTableColumn::Radius:
        validateRadiusInput(row);
        break;
    case AssemblyTableColumn::Thickness:
        validateThicknessInput(row);
        break;
    case AssemblyTableColumn::Material:
        validateMaterialInput(row);
        break;
    default:
        QMessageBox::warning(this,tr("Error"), tr("Input to invalid cell"));
    }

}

void SystemEditorWidget::validateLabelInput(int row)
{
    // half-width alpha numeric symbols

    const int col = AssemblyTableColumn::Label;
    QTableWidgetItem *item = ui->assemblyTable->item(row, col);
    QString text = item->text();

    QRegExp regAlphaNumericSymbol("^[a-zA-Z0-9!-/:-@¥[-`{-~]*$");

    if(regAlphaNumericSymbol.exactMatch(text)){
        opt_sys_->optical_assembly()->surface(row)->set_label(text.toStdString());
    }else{
        QMessageBox::warning(this, "Error", "Invalid label input");
        setContentToCell(ui->assemblyTable, row, col, QString(""));
    }
}

void SystemEditorWidget::validateRadiusInput(int row)
{
    // numbers or infinity

    const int col = AssemblyTableColumn::Radius;
    QTableWidgetItem *item = ui->assemblyTable->item(row, col);
    QString text = item->text();

    // "inf"
    QRegExp reg_inf("([iI]nf)(inity)*");
    if(reg_inf.exactMatch(text)){
        setContentToCell(ui->assemblyTable, row, col, QString("Infinity"));
        opt_sys_->optical_assembly()->surface(row)->profile()->set_cv(0.0);
        return;
    }

    bool isDouble;
    double val = item->data(Qt::EditRole).toDouble(&isDouble);

    if(isDouble){
        if(std::isinf(val)) {
            setContentToCell(ui->assemblyTable, row,col,QString("Infinity"));
            opt_sys_->optical_assembly()->surface(row)->profile()->set_cv(0.0);
        }else{
            opt_sys_->optical_assembly()->surface(row)->profile()->set_radius(val);
        }

    }else{
        QMessageBox::warning(this, tr("Error"), tr("Invalid radius input"));
        setContentToCell(ui->assemblyTable, row,col,QString("Infinity"));
        opt_sys_->optical_assembly()->surface(row)->profile()->set_cv(0.0);
    }
}

void SystemEditorWidget::validateThicknessInput(int row)
{
    // only numbers are valid

    const int col = AssemblyTableColumn::Thickness;
    QTableWidgetItem *item = ui->assemblyTable->item(row, col);
    QString text = item->text();

    bool isDouble;
    double val = text.toDouble(&isDouble);

    if(isDouble){
        if(std::isinf(val)){
            QMessageBox::warning(this, tr("Error"), tr("Infinity for thickness is not supported"));
            setContentToCell(ui->assemblyTable, row, col, 0.0);
            opt_sys_->optical_assembly()->gap(row)->set_thi(0.0);
        }else{
            opt_sys_->optical_assembly()->gap(row)->set_thi(val);
        }
    }else{
        QMessageBox::warning(this, tr("Error"), tr("Invalid thickness input"));
        opt_sys_->optical_assembly()->gap(row)->set_thi(0.0);
    }
}

void SystemEditorWidget::validateMaterialInput(int row)
{
    // search from material library
    // If not found, set AIR

    QRegExp reg_alphanumeric_("[_a-zA-Z0-9¥.¥-]*$");

    int col = AssemblyTableColumn::Material;
    QTableWidgetItem *item = ui->assemblyTable->item(row, col);
    QString text = item->text();

    if(reg_alphanumeric_.exactMatch(text)){
        auto mat = opt_sys_->material_lib()->find(text.toStdString());
        if(mat){ // found in material library
            setContentToCell(ui->assemblyTable, row,col, QString().fromStdString(mat->name()));
            opt_sys_->optical_assembly()->gap(row)->set_material(mat);
            return;
        }else{ // not found
            QMessageBox::warning(this, tr("Error"), "Material not found");
            setContentToCell(ui->assemblyTable, row, col, tr("AIR"));
            opt_sys_->optical_assembly()->gap(row)->set_material(std::make_shared<Air>());
            return;
        }
    }else{ // not alpha numeric input
        QMessageBox::warning(this, tr("Error"), "Invalid material input");
        setContentToCell(ui->assemblyTable, row, col, tr("AIR"));
        opt_sys_->optical_assembly()->gap(row)->set_material(std::make_shared<Air>());
    }

}

void SystemEditorWidget::showSurfacePropertyDlg(int si)
{
    if(si < 0){
        si = ui->assemblyTable->currentRow();
    }

    if(si == ui->assemblyTable->rowCount()-1){
        return;
    }

    SurfacePropertyDialog *dlg = new SurfacePropertyDialog(opt_sys_, si, this);
    dlg->syncUiWithSystem();
    if(dlg->exec() == QDialog::Accepted){
        syncUiWithSystem();
    }

    try {
        delete dlg;
    }  catch (...) {
        qDebug() << "SurfacePropertyDialog delete error";
    }

}



// ******************************************************************************************************************************************
// Spec Tab

void SystemEditorWidget::showContextMenuOnWavelengthTableHeader()
{
    QMenu contextMenu;
    contextMenu.clear();

    QAction *action1 = contextMenu.addAction("Add");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(addWavelength()));

    QAction *action2 = contextMenu.addAction("Remove");
    QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeWavelength()));

    contextMenu.exec(QCursor::pos());
}


void SystemEditorWidget::showContextMenuOnFieldTableHeader()
{
    QMenu contextMenu;
    contextMenu.clear();

    QAction *action1 = contextMenu.addAction("Add");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(addField()));

    QAction *action2 = contextMenu.addAction("Remove");
    QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeField()));

    contextMenu.exec(QCursor::pos());
}

void SystemEditorWidget::addWavelength()
{
    int rowCount = ui->wavelengthTable->rowCount();

    ui->wavelengthTable->insertRow(rowCount);
    int currentRow = ui->wavelengthTable->rowCount()-1;

    setContentToCell(ui->wavelengthTable, currentRow, WavelengthTableColumn::WavelengthValue, SpectralLine::d);
    setContentToCell(ui->wavelengthTable, currentRow, WavelengthTableColumn::WavelengthWeight, 1.0);
    setContentToCell(ui->wavelengthTable, currentRow, WavelengthTableColumn::WavelengthColor, Qt::black);

    updateReferenceWavelengthCombo();
}

void SystemEditorWidget::removeWavelength()
{
    if(ui->wavelengthTable->rowCount() == 1){
        QMessageBox::warning(this, tr("Warining"), tr("At least one component should be set"));
        return;
    }

    int currentRow = ui->wavelengthTable->currentRow();
    ui->wavelengthTable->removeRow(currentRow);

    updateReferenceWavelengthCombo();
}

void SystemEditorWidget::updateReferenceWavelengthCombo()
{
    int origIndex = ui->referenceWavelengthCombo->currentIndex();

    int wvlCount = ui->wavelengthTable->rowCount();
    ui->referenceWavelengthCombo->clear();

    for(int wi = 0; wi < wvlCount; wi++) {
        double value = ui->wavelengthTable->item(wi, WavelengthTableColumn::WavelengthValue)->text().toDouble();
        QString itemText = "W" + QString::number(wi + 1) + ": " + QString::number(value); // W1: 580.60
        ui->referenceWavelengthCombo->addItem(itemText);
    }

    if(ui->referenceWavelengthCombo->count() <= origIndex) {
        ui->referenceWavelengthCombo->setCurrentIndex(origIndex - 1);
    }else{
        ui->referenceWavelengthCombo->setCurrentIndex(origIndex);
    }

}

void SystemEditorWidget::validateCellDoubleClickOnWavelengthTable(int row, int col)
{
    if(col == WavelengthTableColumn::WavelengthValue) {
        QStringList items;
        items << "t" << "s" << "r" << "C" << "C_" << "D" << "d" << "e" << "F" << "F_" << "g" << "h" << "i";

        bool ok;
        QString item = QInputDialog::getItem(this, tr("Input Wavelength"), tr("Select spectral line or input value directly."), items, 0,true, &ok);

        if(ok){
            if(items.contains(item)){ //select item from the list
                double val = SpectralLine::wavelength(item.toStdString());
                setContentToCell(ui->wavelengthTable, row, col, val);
            }else{
                bool isDouble;
                double val = item.toDouble(&isDouble);
                if(isDouble){ // directly input value
                    setContentToCell(ui->wavelengthTable, row, col, val);
                }
            }
        }else {
            QMessageBox::warning(nullptr,"Error","Invalid input.");
        }
    }else if(col == WavelengthTableColumn::WavelengthColor) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            setContentToCell(ui->wavelengthTable, row, col, color);
        }
    }
}

void SystemEditorWidget::addField()
{
    int rowCount = ui->fieldTable->rowCount();

    ui->fieldTable->insertRow(rowCount);
    int currentRow = ui->fieldTable->rowCount()-1;

    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldX, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldY, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldWeight, 1.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldVLX, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldVUX, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldVLY, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldVUY, 0.0);
    setContentToCell(ui->fieldTable, currentRow, FieldTableColumn::FieldColor, Qt::black);
}

void SystemEditorWidget::removeField()
{
    if(ui->fieldTable->rowCount() == 1){
        QMessageBox::warning(this, tr("Warining"), tr("At least one component should be set"));
        return;
    }

    int currentRow = ui->fieldTable->currentRow();
    ui->fieldTable->removeRow(currentRow);
}

void SystemEditorWidget::validateCellDoubleClickOnFieldTable(int row, int col)
{
    if(col == FieldTableColumn::FieldColor) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            setContentToCell(ui->fieldTable, row, col, color);
        }
    }
}
