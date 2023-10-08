#include "LensDataTableModel.h"
#include "SystemEditor/SystemDataConstant.h"
#include <QDebug>
#include <QColor>
#include <QStandardItem>

LensDataTableModel::LensDataTableModel(std::shared_ptr<OpticalSystem> opt_sys, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_opt_sys = opt_sys;
}

LensDataTableModel::~LensDataTableModel()
{
    m_opt_sys = nullptr;
}

void LensDataTableModel::setOpticalSystem(std::shared_ptr<OpticalSystem> opt_sys)
{
    m_opt_sys = opt_sys;
}

int LensDataTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 10;
}

int LensDataTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_opt_sys->GetOpticalAssembly()->NumberOfSurfaces();
}

Qt::ItemFlags LensDataTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled;

    // uneditable
    int i = index.row();
    int j = index.column();
    if(m_opt_sys->GetOpticalAssembly()->GetGap(i)->HasSolve() && j == LensDataColumn::Thickness){
        return QAbstractItemModel::flags(index);
    }

    if(m_opt_sys->GetOpticalAssembly()->GetSurface(i)->HasSolve() && j == LensDataColumn::Radius) {
        return QAbstractItemModel::flags(index);
    }

    if(i == this->rowCount()-1){ // image surface
        return QAbstractItemModel::flags(index);
    }

    return ( QAbstractItemModel::flags(index) | Qt::ItemIsEditable );
}

QVariant LensDataTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())  return QVariant();

    if( index.row() >= m_opt_sys->GetOpticalAssembly()->NumberOfSurfaces() ){
        qDebug() << "Out of surface range";
        return QVariant();
    }

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::DisplayRole || role == Qt::EditRole){

        if( LensDataColumn::Label == j){
            return QString().fromStdString( m_opt_sys->GetOpticalAssembly()->GetSurface(i)->Label());
        }else if (LensDataColumn::SurfaceType == j){
            QString surface_type = QString().fromStdString(m_opt_sys->GetOpticalAssembly()->GetSurface(i)->ProfileName());
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
            return m_opt_sys->GetOpticalAssembly()->GetSurface(i)->Radius();
        }else if (LensDataColumn::Thickness == j){
            return m_opt_sys->GetOpticalAssembly()->GetGap(i)->Thickness();
        }else if( LensDataColumn::Material == j){
            QString material_name = QString().fromStdString(m_opt_sys->GetOpticalAssembly()->GetGap(i)->GetMaterial()->Name());
            if ("AIR" == material_name) {material_name = tr("");}
            return material_name;
        }else if(LensDataColumn::Mode == j){
            return QString().fromStdString(m_opt_sys->GetOpticalAssembly()->GetSurface(i)->InteractMode());
        }else if (LensDataColumn::SemiDiameter == j){
            return m_opt_sys->GetOpticalAssembly()->GetSurface(i)->SemiDiameter();
        }else if (LensDataColumn::Aperture == j){
            QString aperture_shape = QString().fromStdString(m_opt_sys->GetOpticalAssembly()->GetSurface(i)->ApertureShape());
            if(aperture_shape == "None"){
                return tr("");
            }else{
                return aperture_shape;
            }
        }else if(LensDataColumn::ThicknessSolve == j){
            if( m_opt_sys->GetOpticalAssembly()->GetGap(i)->GetSolve()->GetSolveType() > 1 ){
                return QString().fromStdString(m_opt_sys->GetOpticalAssembly()->GetGap(i)->GetSolve()->GetSolveTypeStr());
            }else{
                return tr("");
            }
        }else{
            return QVariant();
        }
    }else if(role == Qt::BackgroundRole){
        QColor gray = QColor(200,200,200);

        // image
        if( i == m_opt_sys->GetOpticalAssembly()->NumberOfSurfaces() - 1 ){
            return gray;
        }
        // solve
        if(m_opt_sys->GetOpticalAssembly()->GetGap(i)->HasSolve() && LensDataColumn::Thickness == j){
            return gray;
        }

    }

    return QVariant();

}

bool LensDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) return false;

    if(role == Qt::EditRole){
        const int i = index.row();
        const int j = index.column();
        if(LensDataColumn::Label == j){
            m_opt_sys->GetOpticalAssembly()->GetSurface(i)->SetLabel(value.toString().toStdString());
        }else if(LensDataColumn::Radius == j){
            m_opt_sys->GetOpticalAssembly()->GetSurface(i)->SetRadius(value.toDouble());
        }else if(LensDataColumn::Thickness == j){
            if( ! m_opt_sys->GetOpticalAssembly()->GetGap(i)->HasSolve()){
                m_opt_sys->GetOpticalAssembly()->GetGap(i)->SetThickness(value.toDouble());
            }
        }else if(LensDataColumn::Material == j){
            auto m = MaterialLibrary::Find(value.toString().toStdString());
            m_opt_sys->GetOpticalAssembly()->GetGap(i)->SetMaterial(m);
        }else if( LensDataColumn::Mode == j){

        }else if(LensDataColumn::SemiDiameter == j){

        }else if(LensDataColumn::Aperture == j){

        }else{

        }

        m_opt_sys->UpdateModel();

        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant LensDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        }else if(section == m_opt_sys->GetOpticalAssembly()->StopIndex()){
            return tr("STO");
        }else if(section == this->rowCount()-1){
            return tr("IMG");
        }else{
            return QVariant(section);
        }
    }
}

bool LensDataTableModel::insertRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginInsertRows(QModelIndex(), row, row+count-1);
    for(auto i = 0; i < count; i++){
        m_opt_sys->GetOpticalAssembly()->Insert(row);
    }
    m_opt_sys->UpdateModel();
    endInsertRows();

    return true;
}

bool LensDataTableModel::removeRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (auto i = 0; i < count; ++i){
        m_opt_sys->GetOpticalAssembly()->Remove(row);
    }
    m_opt_sys->UpdateModel();
    endRemoveRows();
    return true;
}


void LensDataTableModel::setStop(int i)
{
    m_opt_sys->GetOpticalAssembly()->SetStop(i);
    m_opt_sys->UpdateModel();
    emit headerDataChanged(Qt::Vertical, 0, rowCount()-1);
}
