#include "LensDataModel.h"
#include "SystemEditor/SystemDataConstant.h"
#include <QDebug>
#include <QColor>

LensDataModel::LensDataModel(std::shared_ptr<OpticalSystem> opt_sys, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_opt_sys = opt_sys;
}

LensDataModel::~LensDataModel()
{
    m_opt_sys = nullptr;
}

void LensDataModel::setOpticalSystem(std::shared_ptr<OpticalSystem> opt_sys)
{
    m_opt_sys = opt_sys;
}

int LensDataModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 10;
}

int LensDataModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_opt_sys->optical_assembly()->surface_count();
}

Qt::ItemFlags LensDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return ( QAbstractItemModel::flags(index) | Qt::ItemIsEditable );
}

QVariant LensDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    if(index.row() >= m_opt_sys->optical_assembly()->surface_count()){
        qDebug() << "Out of surface range";
        return QVariant();
    }

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::DisplayRole || role == Qt::EditRole){

        if( LensDataColumn::Label == j){
            return QString().fromStdString(m_opt_sys->optical_assembly()->surface(i)->label());
        }else if (LensDataColumn::SurfaceType == j){
            QString surface_type = QString().fromStdString(m_opt_sys->optical_assembly()->surface(i)->profile()->name());
            if(surface_type == "SPH"){
                return tr("Standard");
            }else if(surface_type == "ASP"){
                return tr("EvenAsphere");
            }else if(surface_type == "ODD"){
                return tr("OddAsphere");
            }else{
                return tr("Unknown");
            }
        }else if (LensDataColumn::Radius == j){
            return m_opt_sys->optical_assembly()->surface(i)->profile()->radius();
        }else if (LensDataColumn::Thickness == j){
            return m_opt_sys->optical_assembly()->gap(i)->thi();
        }else if( LensDataColumn::Material == j){
            return QString().fromStdString(m_opt_sys->optical_assembly()->gap(i)->material()->name());
        }else if(LensDataColumn::Mode == j){
            return QString().fromStdString(m_opt_sys->optical_assembly()->surface(i)->interact_mode());
        }else if (LensDataColumn::SemiDiameter == j){
            return m_opt_sys->optical_assembly()->surface(i)->semi_diameter();
        }else if (LensDataColumn::Aperture == j){
            QString aperture_shape = QString().fromStdString(m_opt_sys->optical_assembly()->surface(i)->aperture_shape());
            if(aperture_shape == "None"){
                return tr("");
            }else{
                return aperture_shape;
            }
        }else{
            return QVariant();
        }
    }else if(role == Qt::BackgroundRole){
        // colorize cells of radius or thickness with solve
        //return QColor(0,0,255);

    }

    return QVariant();

}

bool LensDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) return false;

    if(role == Qt::EditRole){
        const int i = index.row();
        const int j = index.column();
        if(LensDataColumn::Label == j){
            m_opt_sys->optical_assembly()->surface(i)->set_label(value.toString().toStdString());
        }else if(LensDataColumn::Radius == j){
            m_opt_sys->optical_assembly()->surface(i)->profile()->set_radius(value.toDouble());
        }else if(LensDataColumn::Thickness == j){
            m_opt_sys->optical_assembly()->gap(i)->set_thi(value.toDouble());
        }else if(LensDataColumn::Material == j){
            auto m = MaterialLibrary::find(value.toString().toStdString());
            m_opt_sys->optical_assembly()->gap(i)->set_material(m);
        }else if( LensDataColumn::Mode == j){

        }else if(LensDataColumn::SemiDiameter == j){

        }else if(LensDataColumn::Aperture == j){

        }else{

        }

        m_opt_sys->update_model();

        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant LensDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }
    if(orientation == Qt::Horizontal) {
        switch (section) {
        case LensDataColumn::Label:
            return tr("Label");
        case LensDataColumn::SurfaceType:
            return tr("Type");
        case LensDataColumn::Radius:
            return tr("Radius");
        case LensDataColumn::Thickness:
            return tr("Thickness");
        case LensDataColumn::Material:
            return tr("Material");
        case LensDataColumn::Mode:
            return tr("Mode");
        case LensDataColumn::SemiDiameter:
            return tr("SemiDiameter");
        case LensDataColumn::Aperture:
            return tr("Aperture");
        default:
            return QVariant();
        }
    } else { // Vertical
        if(section == 0){
            return tr("OBJ");
        }else if(section == m_opt_sys->optical_assembly()->stop_index()){
            return tr("STO");
        }else if(section == this->rowCount()-1){
            return tr("IMG");
        }else{
            return QVariant(section);
        }
    }
}

bool LensDataModel::insertRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginInsertRows(QModelIndex(), row, row+count-1);
    for(auto i = 0; i < count; i++){
        m_opt_sys->optical_assembly()->insert(row);
    }
    m_opt_sys->update_model();
    endInsertRows();

    return true;
}

bool LensDataModel::removeRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (auto i = 0; i < count; ++i){
        m_opt_sys->optical_assembly()->remove(row);
    }
    m_opt_sys->update_model();
    endRemoveRows();
    return true;
}

void LensDataModel::appendData()
{
    int n = m_opt_sys->optical_assembly()->surface_count();
    beginInsertRows(QModelIndex(), n, n);
    m_opt_sys->optical_assembly()->add_surface_and_gap();
    endInsertRows();
}

void LensDataModel::setStop(int i)
{
    m_opt_sys->optical_assembly()->set_stop(i);
    m_opt_sys->update_model();
    emit headerDataChanged(Qt::Vertical, 0, rowCount()-1);
}
