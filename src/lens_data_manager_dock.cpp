#include "lens_data_manager_dock.h"

#include "surface_property_dialog.h"

#include "float_delegate.h"

#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QToolBar>
#include <QDebug>


#include <fstream>
#include <memory>

LensDataManagerDock::LensDataManagerDock(std::shared_ptr<OpticalModel> opt_model, QString label, QWidget *parent) :
    ads::CDockWidget(label, parent),
    opt_model_(opt_model)
{
    // this dock is never closed
    this->setFeature(CDockWidget::DockWidgetClosable, false);

    // table
    table_ = new QTableWidget;
    this->setWidget(table_);
    QObject::connect(table_, SIGNAL(cellChanged(int,int)), this, SLOT(validateCellInput(int,int)));
    //QObject::connect(table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(validateCellDoubleClicked(int,int)));
    QObject::connect(table_, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(validateItemDoubleClicked(QTableWidgetItem*)));

    FloatDelegate *delegate = new FloatDelegate(4, table_);
    table_->setItemDelegate(delegate);


    // toolbar
    toolbar_ = this->createDefaultToolBar();
    auto actionSpec = toolbar_->addAction("Spec");

    // spec setting
    specSettingDlg_ = std::make_unique<SpecSettingDialog>(opt_model_, this);
    QObject::connect(actionSpec, SIGNAL(triggered()), this, SLOT(showSpecSettingDlg()));

    // context menu
    table_->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(table_->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnHeader()));

    table_->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(table_, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnCell()));

    initialize();

    connectValidateCell(true);

}

LensDataManagerDock::~LensDataManagerDock()
{
    opt_model_.reset();
    delete table_;
    delete toolbar_;
}


void LensDataManagerDock::initialize()
{
    table_->clear();

    QStringList hHeaderLabels = {"Label", "Surface Type", "Radius", "Thickness", "Material", "Mode", "SemiDiameter", "Aperture", "MaxAperture"};
    table_->setColumnCount(hHeaderLabels.size());
    table_->setHorizontalHeaderLabels(hHeaderLabels);

    table_->setRowCount(2);
    this->updateVerticalHeader();
}

void LensDataManagerDock::setOpticalModel(std::shared_ptr<OpticalModel> opm)
{
    opt_model_.reset();
    opt_model_ = opm;
}

void LensDataManagerDock::syncTableWithModel()
{
    connectValidateCell(false);

    this->initialize();

    int num_srf = opt_model_->seq_model()->surface_count();
    table_->setCurrentCell(0,0);

    table_->setRowCount(num_srf);

    // make image surface thickness and material uneditable
    //int last_row = table_->rowCount() -1;
    //setCellEditable(last_row, Column::Thickness, false);
    //setCellEditable(last_row, Column::Material, false);

    // s0..sI
    for(int i = 0; i < num_srf; i++){
        syncRowWithModel(i);
    }

    //sI
    //setValueToCell(num_srf-1, Column::Radius, "Infinity");

    updateVerticalHeader();

    connectValidateCell(true);

    table_->update();


}


void LensDataManagerDock::syncRowWithModel(int row)
{
    int image_surface_index = opt_model_->seq_model()->surface_count() - 1;

    if(row < image_surface_index){
        std::string surface_label = opt_model_->seq_model()->surface(row)->label();
        double r                  = opt_model_->seq_model()->surface(row)->profile()->radius();
        double thi                = opt_model_->seq_model()->gap(row)->thi();
        std::string medium_name   = opt_model_->seq_model()->gap(row)->medium()->display_name();
        std::string surface_type  = opt_model_->seq_model()->surface(row)->profile()->name();
        std::string interact_mode = opt_model_->seq_model()->surface(row)->interact_mode();
        double sd                 = opt_model_->seq_model()->surface(row)->semi_diameter();
        double max_ap             = opt_model_->seq_model()->surface(row)->max_aperture();
        std::string aperture_type = opt_model_->seq_model()->surface(row)->aperture_shape();

        setValueToCell(row, Column::Label, QString().fromStdString(surface_label));
        setValueToCell(row, Column::Radius, r);
        setValueToCell(row, Column::Thickness, thi);
        setValueToCell(row, Column::Material, QString().fromStdString(medium_name));
        setValueToCell(row, Column::Mode, QString().fromStdString(interact_mode));
        setValueToCell(row, Column::SurfaceType, QString().fromStdString(surface_type));
        setValueToCell(row, Column::SemiDiameter, sd);
        setValueToCell(row, Column::ApertureShape, QString().fromStdString(aperture_type));
        setValueToCell(row, Column::MaxAperture, max_ap);

        updateRowColor(row);

        if(row == 0){
            setCellEditable(row, Column::Label, false);
            //setCellEditable(row, Column::Radius, false);
            //setCellEditable(row, Column::Thickness, false);
            //setCellEditable(row, Column::Material, false);
            setCellEditable(row, Column::Mode, false);
            setCellEditable(row, Column::SurfaceType, false);
            setCellEditable(row, Column::SemiDiameter, false);
            setCellEditable(row, Column::ApertureShape, false);
            setCellEditable(row, Column::MaxAperture, false);
        }


    }else if(row == image_surface_index){

        std::string surface_label = opt_model_->seq_model()->surface(row)->label();
        double r                  = opt_model_->seq_model()->surface(row)->profile()->radius();
        std::string interact_mode = opt_model_->seq_model()->surface(row)->interact_mode();
        double sd                 = opt_model_->seq_model()->surface(row)->semi_diameter();
        std::string surface_type  = opt_model_->seq_model()->surface(row)->profile()->name();

        setValueToCell(row, Column::Label, QString().fromStdString(surface_label));
        setValueToCell(row, Column::Radius, r);
        setValueToCell(row, Column::Mode, QString().fromStdString(interact_mode));
        setValueToCell(row, Column::SurfaceType, QString().fromStdString(surface_type));
        setValueToCell(row, Column::SemiDiameter, sd);

        setCellEditable(row, Column::Label, false);
        setCellEditable(row, Column::Radius, false);
        setCellEditable(row, Column::Thickness, false);
        setCellEditable(row, Column::Material, false);
        setCellEditable(row, Column::Mode, false);
        setCellEditable(row, Column::SurfaceType, false);
        setCellEditable(row, Column::SemiDiameter, false);
        setCellEditable(row, Column::ApertureShape, false);
        setCellEditable(row, Column::MaxAperture, false);
    }

}

/******************************************************************************************************************
 * Spec Setting
 * ***************************************************************************************************************/
void LensDataManagerDock::showSpecSettingDlg()
{
    specSettingDlg_->setOpticalModel(opt_model_);
    specSettingDlg_->syncUiWithModel();
    if( specSettingDlg_->exec() == QDialog::Accepted) {
        opt_model_->update_model();
        syncTableWithModel();
    }
}


/***********************************************************************************************************************
 * Surface Property
 * *********************************************************************************************************************/
void LensDataManagerDock::showSurfacePropertyDlg(int si)
{
    if(si < 0){
        si = table_->currentRow();
    }

    if(si == table_->rowCount()-1){
        return;
    }

    SurfacePropertyDialog *dlg = new SurfacePropertyDialog(opt_model_, si, this);
    dlg->syncUiWithModel();
    if(dlg->exec() == QDialog::Accepted){
        syncRowWithModel(si);
    }

    try {
        delete dlg;
    }  catch (...) {
        qDebug() << "SurfacePropertyDialog delete error";
    }

}



/***************************************************************************************************************
 * Context menu
 * **************************************************************************************************************/

void LensDataManagerDock::showContextMenuOnCell()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Property");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

    contextMenu.exec(QCursor::pos());
}

void LensDataManagerDock::showContextMenuOnHeader()
{
    QMenu contextMenu;
    contextMenu.clear();


    // actions in context menu
    if(table_->currentRow() == 0){ // Obj
        QAction *action2 = contextMenu.addAction("Insert after");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertAfter()));
    }
    else if(table_->currentRow() == table_->rowCount()-1){ // Img
        QAction *action1 = contextMenu.addAction("Insert before");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertBefore()));
    }
    else{
        QAction *action1 = contextMenu.addAction("Insert before");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertBefore()));

        QAction *action2 = contextMenu.addAction("Insert after");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertAfter()));

        QAction *action3 = contextMenu.addAction("Remove");
        QObject::connect(action3, SIGNAL(triggered()), this, SLOT(remove()));
    }

    contextMenu.exec(QCursor::pos());
}

void LensDataManagerDock::insertBefore()
{
    int currentRow = table_->currentRow();

    insertRow(currentRow);
    updateVerticalHeader();
}

void LensDataManagerDock::insertAfter()
{
    int currentRow = table_->currentRow();

    insertRow(currentRow + 1);
    updateVerticalHeader();
}

void LensDataManagerDock::remove()
{
    int currentRow = table_->currentRow();
    table_->removeRow(currentRow);
    opt_model_->seq_model()->remove(currentRow);

    updateVerticalHeader();
}

void LensDataManagerDock::insertRow(int row)
{
    connectValidateCell(false);

    if(table_->rowCount() == 0){
        table_->setRowCount(1);
    }else{
        table_->insertRow(row);
    }

    // set header item with surface number
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(QString::number(row));
    table_->setVerticalHeaderItem(row,item);


    // add surface to optical model
    //opt_model_->seq_model()->insert(row-1);
    opt_model_->seq_model()->insert(row);
    opt_model_->update_model();


    // input cell values
    syncRowWithModel(row);

    connectValidateCell(true);
}

void LensDataManagerDock::updateVerticalHeader()
{
    int rowCount = table_->rowCount();

    // surface number
    for(int i = 0; i < rowCount; i++)
    {
        QTableWidgetItem *item = table_->verticalHeaderItem(i);
        if(!item){
            item = new QTableWidgetItem;
            table_->setVerticalHeaderItem(i, item);
        }
        table_->verticalHeaderItem(i)->setText(QString::number(i));
    }

    // OBJ, IMG, STO
    table_->verticalHeaderItem(0)->setText("OBJ");
    table_->verticalHeaderItem(rowCount - 1)->setText("IMG");
    if( rowCount > 2 ){
        if(opt_model_){
            stop_surface_ = opt_model_->seq_model()->stop_surface();
            table_->verticalHeaderItem(stop_surface_)->setText("STO");
        }
    }
}



/******************************************************************************************************
 * Cell operations
 * ****************************************************************************************************/
void LensDataManagerDock::setValueToCell(int row, int col, double val)
{
    QTableWidgetItem *item = table_->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table_->setItem(row, col, item);
    }

    item->setTextAlignment(Qt::AlignRight);

    if(std::isinf(val)){
        item->setText("Infinity");
    }
    else{
        item->setText(QString::number(val, 'f', 10));
    }

}


void LensDataManagerDock::setValueToCell(int row, int col, QString str)
{
    QTableWidgetItem *item = table_->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table_->setItem(row, col, item);
    }
    item->setTextAlignment(Qt::AlignRight);
    item->setText(str);
}

void LensDataManagerDock::validateCellDoubleClicked(int row, int col)
{
    switch (col) {
    case Column::SurfaceType:
    case Column::ApertureShape:
    case Column::MaxAperture:
    case Column::Mode:
    case Column::SemiDiameter:
        showSurfacePropertyDlg(row);
        break;
    }

}


void LensDataManagerDock::validateItemDoubleClicked(QTableWidgetItem* item)
{
    if(item){
        /*
        switch (item->column()) {
        case Column::SurfaceType:
        case Column::ApertureShape:
        case Column::MaxAperture:
        case Column::Mode:
        case Column::SemiDiameter:
            showSurfacePropertyDlg(item->row());
            break;
        }
        */
        validateCellDoubleClicked(item->row(), item->column());
    }

}

void LensDataManagerDock::validateCellInput(int row, int col)
{
    bool ok = true;

    switch (col) {
    case Column::Label:
        ok = validateLabelInput(row);
        break;
    case Column::Radius:
        ok = validateRadiusInput(row);
        break;
    case Column::Thickness:
        if(row < table_->rowCount()-1){
            ok = validateThicknessInput(row);
        }
        break;
    case Column::Material:
        if(row < table_->rowCount()-1){
            ok = validateMediumInput(row);
        }
        break;
    default:
        ok = true;
    }

    if(!ok){
        QMessageBox::warning(this,tr("Error"), tr("Invalid input"));
    }else{
        qDebug() << "Input: " << "(" << row << ", " << col << ")";
    }

}

bool LensDataManagerDock::validateLabelInput(int row)
{
    int col = Column::Label;
    QTableWidgetItem *item = table_->item(row, col);
    QString text = item->text();

    //QRegExp reg_alphanumeric_("([_a-zA-Z0-9¥.¥-¥(¥)])*$");
    QRegExp reg_all_char(".*$");

    if(reg_all_char.exactMatch(text)){
        opt_model_->seq_model()->surface(row)->set_label(text.toStdString());
        return true;
    }else{
        setValueToCell(row,col,"");
        //item->setText("");
        return false;
    }
}

bool LensDataManagerDock::validateRadiusInput(int row)
{
    int col = Column::Radius;
    QTableWidgetItem *item = table_->item(row, col);
    QString text = item->text();

    QRegExp reg_inf("([iI]nf)(inity)*");

    if(reg_inf.exactMatch(text)){
        //item->setText("Infinity");
        setValueToCell(row,col,"Infinity");
        opt_model_->seq_model()->surface(row)->profile()->set_cv(0.0);
        return true;
    }

    bool isDouble;
    double val = item->data(Qt::EditRole).toDouble(&isDouble);
    //double val = text.toDouble(&isDouble);

    qDebug() << "Radius Input: text= " << text << ", val= " << val;

    if(isDouble){
        if(std::isinf(val)) {
            //item->setText("Infinity");
            setValueToCell(row,col,"Infinity");
            opt_model_->seq_model()->surface(row)->profile()->set_cv(0.0);
        }else{
            opt_model_->seq_model()->surface(row)->profile()->set_radius(val);
        }
        return true;
    }else{
        //item->setText("Infinity");
        setValueToCell(row,col,"Infinity");
        opt_model_->seq_model()->surface(row)->profile()->set_cv(0.0);
        return false;
    }
}

bool LensDataManagerDock::validateThicknessInput(int row)
{
    int col = Column::Thickness;
    QTableWidgetItem *item = table_->item(row, col);
    QString text = item->text();

    QRegExp reg_inf("([iI]nf)(inity)*");

    if(reg_inf.exactMatch(text)){
        //item->setText("Infinity");
        setValueToCell(row,col,"Infinity");
        opt_model_->seq_model()->gap(row)->set_thi(std::numeric_limits<double>::infinity());
        return true;
    }

    bool isDouble;
    double val = text.toDouble(&isDouble);

    if(isDouble){
        if(std::isinf(val)){
            //item->setText("Infinity");
            setValueToCell(row,col,"Infinity");
            opt_model_->seq_model()->gap(row)->set_thi(std::numeric_limits<double>::infinity());
        }else{
            opt_model_->seq_model()->gap(row)->set_thi(val);
        }
        return true;
    }else{
        //item->setText(QString::number(0.0));
        setValueToCell(row,col,0.0);
        opt_model_->seq_model()->gap(row)->set_thi(0.0);
        return false;
    }
}

bool LensDataManagerDock::validateMediumInput(int row)
{
    QRegExp reg_alphanumeric_("[_a-zA-Z0-9¥.¥-]*$");
    QRegExp reg_air("[aA][iI][rR]*$");

    int col = Column::Material;
    QTableWidgetItem *item = table_->item(row, col);
    QString text = item->text();

    if(reg_alphanumeric_.exactMatch(text)){
        if(reg_air.exactMatch(text)){ // input "air", "AIR", "Air", etc
            //item->setText("AIR");
            setValueToCell(row,col,"AIR");
            opt_model_->seq_model()->gap(row)->set_medium(std::make_shared<Air>());
            return true;
        }else{
            auto med = opt_model_->database()->find(text.toStdString());
            if(med){
                //item->setText(QString().fromStdString(med->display_name()));
                setValueToCell(row,col,QString().fromStdString(med->display_name()));
                opt_model_->seq_model()->gap(row)->set_medium(med);
                return true;
            }else{
                //item->setText("AIR");
                setValueToCell(row,col,"AIR");
                opt_model_->seq_model()->gap(row)->set_medium(std::make_shared<Air>());
                return false;
            }
        }

    }else{
        //item->setText("AIR");
        setValueToCell(row,col,"AIR");
        opt_model_->seq_model()->gap(row)->set_medium(std::make_shared<Air>());
        return false;
    }

    return true;
}


void LensDataManagerDock::setCellEditable(int row, int col, bool state)
{
    QTableWidgetItem* item = table_->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table_->setItem(row, col,item);
    }

    QColor white_smoke(245,245,245);

    if(state){
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setBackground(Qt::white);
    }else{
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setBackground(white_smoke);
    }
}


void LensDataManagerDock::updateRowColor(int row)
{
    // https://www.webcreatorbox.com/webinfo/color-name
    QColor light_yellow(255, 255, 224);
    QColor light_cyan(224, 255, 255);

    QString surface_type = table_->item(row, Column::SurfaceType)->text();
    if("ASP" == surface_type){
        setColorToCell(row, Column::SurfaceType, light_cyan);
    }else{
        setColorToCell(row, Column::SurfaceType, Qt::white);
    }

    QString aperture_type = table_->item(row, Column::ApertureShape)->text();
    if("Circular" == aperture_type){
        setColorToCell(row, Column::ApertureShape, light_yellow);
        setColorToCell(row, Column::MaxAperture,   light_yellow);
    }else if("Rectangle" == aperture_type){

    }else{
        setColorToCell(row, Column::ApertureShape, Qt::white);
        setColorToCell(row, Column::MaxAperture,   Qt::white);
    }

}

void LensDataManagerDock::setColorToCell(int row, int col, QColor color)
{
    QTableWidgetItem* item = table_->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table_->setItem(row, col, item);
    }
    item->setBackground(QBrush(color));
}

void LensDataManagerDock::connectValidateCell(bool isConnected)
{
    if(isConnected){
        QObject::connect(table_, SIGNAL(cellChanged(int,int)), this, SLOT(validateCellInput(int,int)));
    }else{
        QObject::disconnect(table_, SIGNAL(cellChanged(int,int)), this, SLOT(validateCellInput(int,int)));
    }
}
