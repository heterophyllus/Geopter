#include "LensSpreadSheet.h"
#include "SpreadSheetItem.h"
#include "FloatDelegate.h"
#include "OneByteDelegate.h"
#include "SurfacePropertyDlg.h"
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

LensSpreadSheet::LensSpreadSheet(std::shared_ptr<OpticalSystem> optsys, QWidget *parent)
    : QTableWidget(parent), m_opticalSystem(optsys), m_displayDigit(4), m_isUserEdit(false)
{
    QStringList hHeaderLabels = {"Label", "Surface Type", "Radius", "Thickness", "Material", "Mode", "SemiDiameter", "Aperture"};
    this->setColumnCount(hHeaderLabels.size());
    this->setHorizontalHeaderLabels(hHeaderLabels);
    this->setRowCount(3);
    this->postProcess();
    this->setSortingEnabled(false);

    this->setItemDelegateForColumn(LensSpreadSheet::Label,        new OneByteDelegate(this));
    this->setItemDelegateForColumn(LensSpreadSheet::Radius,       new FloatDelegate(m_displayDigit, true, this)); // allow inf
    this->setItemDelegateForColumn(LensSpreadSheet::Thickness,    new FloatDelegate(m_displayDigit, false, this));
    this->setItemDelegateForColumn(LensSpreadSheet::Material,     new OneByteDelegate(this));
    this->setItemDelegateForColumn(LensSpreadSheet::SemiDiameter, new FloatDelegate(m_displayDigit, false, this));

    this->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->verticalHeader(),  SIGNAL(customContextMenuRequested(const QPoint &)), this,  SLOT(showContextMenuOnHeader()) );

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnCell()));

    QObject::connect(this,  SIGNAL(itemChanged(QTableWidgetItem*)), this,  SLOT(applyChange(QTableWidgetItem*)) );

    this->reload();
}

void LensSpreadSheet::postProcess()
{
    for(int i = 0; i < this->rowCount(); i++) {
        for(int j = 0; j < this->columnCount(); j++) {
            QTableWidgetItem * item = this->item(i, j);
            if(!item){
                this->setItem(i, j, new SpreadSheetItem() );
                item = this->item(i, j);
            }

            // set uneditable to some column
            switch (j) {
            case LensSpreadSheet::SurfaceType:
            case LensSpreadSheet::Mode:
            case LensSpreadSheet::SemiDiameter:
            case LensSpreadSheet::Aperture:
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                break;
            }

        }
    }
}

void LensSpreadSheet::showContextMenuOnHeader()
{
    int currentRow = this->currentRow();
    int imageSurfaceIndex = this->rowCount() - 1;

    if(currentRow == 0){ // at object surface

    }else if(currentRow == imageSurfaceIndex){ //at image surface
        QMenu contextMenu;

        QAction *action1 = contextMenu.addAction("Insert");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertSurface()));

        contextMenu.exec(QCursor::pos());
    }else{
        QMenu contextMenu;

        QAction *action1 = contextMenu.addAction("Insert");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertSurface()));

        QAction *action2 = contextMenu.addAction("Remove");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeSurface()));

        QAction *action3 = contextMenu.addAction("Set stop");
        QObject::connect(action3, SIGNAL(triggered()), this, SLOT(setStop()));

        QAction *action4 = contextMenu.addAction("Property");
        QObject::connect(action4, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

        contextMenu.exec(QCursor::pos());
    }
}

void LensSpreadSheet::showContextMenuOnCell()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Property");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

    contextMenu.exec(QCursor::pos());
}

void LensSpreadSheet::showSurfacePropertyDlg()
{
    int si = this->currentRow();

    if( 0 > si || si > (this->rowCount()-1) ){
        qDebug() << "LensSpreadSheet::showSurfacePropertyDlg()  out of surface index si= " << si;
        return;
    }

    SurfacePropertyDlg dlg(m_opticalSystem, si, this);
    dlg.syncUiWithSystem();
    if(dlg.exec() == QDialog::Accepted){
        updateUiSurfaceData(si);
    }
}

void LensSpreadSheet::insertSurface()
{
    int row = this->currentRow();

    m_opticalSystem->optical_assembly()->insert_surface(row);

    this->insertRow(row);
    this->postProcess();
    this->updateUiSurfaceData(row);
}

void LensSpreadSheet::removeSurface()
{
    int row = this->currentRow();
    this->removeRow(row);

    m_opticalSystem->optical_assembly()->remove(row);
    m_opticalSystem->update_model();

    this->reload();
}

void LensSpreadSheet::setStop()
{
    int s = this->currentRow();
    m_opticalSystem->optical_assembly()->set_stop(s);
    m_opticalSystem->update_model();
    this->reload();
}

void LensSpreadSheet::updateUiSurfaceData(int si)
{
    std::string surfaceLabel = m_opticalSystem->optical_assembly()->surface(si)->label();
    double radius            = m_opticalSystem->optical_assembly()->surface(si)->profile()->radius();
    double thi               = m_opticalSystem->optical_assembly()->gap(si)->thi();
    std::string materialName = m_opticalSystem->optical_assembly()->gap(si)->material()->name();
    std::string surfaceType  = m_opticalSystem->optical_assembly()->surface(si)->profile()->name();
    std::string interactMode = m_opticalSystem->optical_assembly()->surface(si)->interact_mode();
    double sd                = m_opticalSystem->optical_assembly()->surface(si)->semi_diameter();
    //double max_ap            = m_opticalSystem->optical_assembly()->surface(si)->max_aperture();
    std::string apertureType = m_opticalSystem->optical_assembly()->surface(si)->aperture_shape();

    if(surfaceType == "SPH"){
        surfaceType = "Standard";
    }else if(surfaceType == "ASP"){
        surfaceType = "EvenAsphere";
    }else if(surfaceType == "ODD"){
        surfaceType = "OddAsphere";
    }

    if(apertureType == "None"){
        apertureType = "";
    }

    setValue(si, LensSpreadSheet::Label,        QString().fromStdString(surfaceLabel));
    setValue(si, LensSpreadSheet::SurfaceType,  QString().fromStdString(surfaceType));
    setValue(si, LensSpreadSheet::Radius,       radius);
    setValue(si, LensSpreadSheet::Thickness,    thi);
    setValue(si, LensSpreadSheet::Material,     QString().fromStdString(materialName));
    setValue(si, LensSpreadSheet::Mode,         QString().fromStdString(interactMode));
    setValue(si, LensSpreadSheet::SemiDiameter, sd);
    setValue(si, LensSpreadSheet::Aperture,     QString().fromStdString(apertureType));
}

void LensSpreadSheet::setValue(int row, int col, QVariant value, int role)
{
    SpreadSheetItem* item = dynamic_cast<SpreadSheetItem*>( this->item(row, col) );
    item->setData(role, value);
}

void LensSpreadSheet::reload()
{
    m_isUserEdit = false;
    int surfaceCount = m_opticalSystem->optical_assembly()->surface_count();
    this->setRowCount(surfaceCount);
    this->postProcess();

    for(int si = 0; si < surfaceCount; si++) {
        updateUiSurfaceData(si);
    }

    setupVerticalHeader();
    m_isUserEdit = true;
}

void LensSpreadSheet::applyChange(QTableWidgetItem* item)
{
    if( !m_isUserEdit) return;

    int row = item->row();
    int col = item->column();
    int si = row;

    if(LensSpreadSheet::Label == col){
        std::string label = item->data(Qt::EditRole).toString().toStdString();
        m_opticalSystem->optical_assembly()->surface(si)->set_label(label);
    }
    else if(LensSpreadSheet::Radius == col){
        double radius = item->data(Qt::EditRole).toDouble();
        m_opticalSystem->optical_assembly()->surface(si)->profile()->set_radius(radius);
    }
    else if(LensSpreadSheet::Thickness == col){
        double thi = item->data(Qt::EditRole).toDouble();
        m_opticalSystem->optical_assembly()->gap(si)->set_thi(thi);
    }
    else if(LensSpreadSheet::Material == col){
        std::string matName = item->data(Qt::EditRole).toString().toStdString();
        auto mat = m_opticalSystem->material_lib()->find(matName);

        if(!mat){ // material not found in the library
            QMessageBox::warning(this, tr("Error"), "Invalid material");
        }

        m_opticalSystem->optical_assembly()->gap(si)->set_material(mat);
    }
    else{

    }

    // in order to update semidiameter etc, sync ui
    m_opticalSystem->update_model();
    this->reload();
}


void LensSpreadSheet::setupVerticalHeader()
{
    const int surfaceCount = m_opticalSystem->optical_assembly()->surface_count();
    const int image_index = m_opticalSystem->optical_assembly()->image_index();
    const int stop = m_opticalSystem->optical_assembly()->stop_index();

    Q_ASSERT(surfaceCount == (this->rowCount()));

    QStringList vHeaderLabels;

    for(int si = 0; si < surfaceCount; si++){
        if(si == 0){
            vHeaderLabels << "OBJ";
        }else if(si == image_index){
            vHeaderLabels << "IMG";
        }else if(si == stop){
            vHeaderLabels << "STO";
        }else{
            vHeaderLabels << QString::number(si);
        }
    }

    this->setVerticalHeaderLabels(vHeaderLabels);

}
