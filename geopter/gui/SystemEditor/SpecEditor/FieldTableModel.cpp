#include "SystemEditor/SpecEditor/FieldTableModel.h"
#include "SystemEditor/SystemDataConstant.h"

FieldTableModel::FieldTableModel(QObject* parent) :
    QAbstractTableModel(parent)
{
    m_fields.clear();
}


int FieldTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 8;
}

int FieldTableModel::rowCount(const QModelIndex &) const
{
    return m_fields.size();
}

Qt::ItemFlags FieldTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return ( QAbstractItemModel::flags(index) | Qt::ItemIsEditable );
}

QVariant FieldTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::EditRole || role == Qt::DisplayRole){
        switch (j) {
        case static_cast<int>(FieldTableColumn::X):
            return m_fields[i].x;
        case static_cast<int>(FieldTableColumn::Y):
            return m_fields[i].y;
        case static_cast<int>(FieldTableColumn::Weight):
            return m_fields[i].wt;
        case static_cast<int>(FieldTableColumn::VUX):
            return m_fields[i].vux;
        case static_cast<int>(FieldTableColumn::VUY):
            return m_fields[i].vuy;
        case static_cast<int>(FieldTableColumn::VLX):
            return m_fields[i].vlx;
        case static_cast<int>(FieldTableColumn::VLY):
            return m_fields[i].vly;
        //case static_cast<int>(FieldTableColumn::Color):
        //    return m_fields[i].color;
        default:
            return QVariant();
        }
    }else if(role == Qt::BackgroundRole || role == Qt::DecorationRole){
        if(j == static_cast<int>(FieldTableColumn::Color)){
            return m_fields[i].color;
        }
    }

    return QVariant();
}

QVariant FieldTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    if(orientation == Qt::Horizontal){
        switch (section) {
        case static_cast<int>(FieldTableColumn::X):
            return tr("X");
        case static_cast<int>(FieldTableColumn::Y):
            return tr("Y");
        case static_cast<int>(FieldTableColumn::Weight):
            return tr("Weight");
        case static_cast<int>(FieldTableColumn::VUX):
            return tr("VUX");
        case static_cast<int>(FieldTableColumn::VUY):
            return tr("VUY");
        case static_cast<int>(FieldTableColumn::VLX):
            return tr("VLX");
        case static_cast<int>(FieldTableColumn::VLY):
            return tr("VLU");
        case static_cast<int>(FieldTableColumn::Color):
            return tr("Color");
        default:
            return QVariant();
        }
    }else { // Vertical
        return QVariant(section);
    }

}

bool FieldTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) return false;

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::EditRole){

        if(j == static_cast<int>(FieldTableColumn::X)){
            m_fields[i].x = value.toDouble();
        }else if(j == static_cast<int>(FieldTableColumn::Y)){
            m_fields[i].y = value.toDouble();
        }else if(j == static_cast<int>(FieldTableColumn::Weight)){
            m_fields[i].wt = value.toDouble();
        }else if(j == static_cast<int>(FieldTableColumn::VUX)){
            m_fields[i].vux = value.toDouble();
        }else if(j == static_cast<int>(FieldTableColumn::VUY)){
            m_fields[i].vuy = value.toDouble();
        }else if(j== static_cast<int>(FieldTableColumn::VLX)){
            m_fields[i].vlx = value.toDouble();
        }else if(j == static_cast<int>(FieldTableColumn::VLY)){
            m_fields[i].vly = value.toDouble();
        } else if(j == static_cast<int>(FieldTableColumn::Color)){
            m_fields[i].color = value.value<QColor>();
        }

        emit dataChanged(index, index);
        return true;

    }else if(role == Qt::BackgroundRole){
        m_fields[i].color = value.value<QColor>();
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void FieldTableModel::setData(const std::shared_ptr<OpticalSystem> opt_sys)
{
    m_fields.clear();

    const int num_flds = opt_sys->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();

    beginInsertRows(QModelIndex(), 0, num_flds-1);

    for(int fi = 0; fi < num_flds; fi++){
        Field* fld = opt_sys->GetOpticalSpec()->GetFieldSpec()->GetField(fi);

        FieldData fld_data;
        fld_data.x = fld->X();
        fld_data.y = fld->Y();
        fld_data.wt = fld->Weight();
        fld_data.vlx = fld->VLX();
        fld_data.vly = fld->VLY();
        fld_data.vux = fld->VUX();
        fld_data.vuy = fld->VUY();
        fld_data.color = rgbToQColor(fld->RenderColor());

        m_fields.append(fld_data);
    }

    endInsertRows();
    emit dataChanged(this->index(0,0), this->index(m_fields.size()-1, 7));
}

bool FieldTableModel::insertRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginInsertRows(QModelIndex(), row, row+count-1);
    for(int i = 0; i < count; i++){
        m_fields.insert(row, FieldData());
    }
    endInsertRows();

    return true;
}

bool FieldTableModel::removeRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; i++){
        m_fields.remove(row);
    }
    endRemoveRows();
    return true;
}

void FieldTableModel::addRow()
{
    beginInsertRows(QModelIndex(), m_fields.count(), m_fields.count());
    m_fields.append(FieldData());
    endInsertRows();
    emit dataChanged(this->index(0,0), this->index(m_fields.size()-1, 7));
}

void FieldTableModel::applyData(std::shared_ptr<OpticalSystem> opt_sys)
{
    opt_sys->GetOpticalSpec()->GetFieldSpec()->clear();

    for(int fi = 0; fi < m_fields.size(); fi++){
        double x = m_fields[fi].x;
        double y = m_fields[fi].y;
        double wt = m_fields[fi].wt;
        double vlx = m_fields[fi].vlx;
        double vly = m_fields[fi].vly;
        double vux = m_fields[fi].vux;
        double vuy = m_fields[fi].vuy;
        Rgb color = QColorToRgb(m_fields[fi].color);

        opt_sys->GetOpticalSpec()->GetFieldSpec()->AddField(x,y,wt,color,vuy, vly, vux, vlx);
    }
}

Rgb FieldTableModel::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}

QColor FieldTableModel::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}
