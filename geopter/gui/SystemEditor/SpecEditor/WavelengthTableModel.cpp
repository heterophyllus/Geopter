#include "WavelengthTableModel.h"
#include "SystemEditor/SystemDataConstant.h"

WavelengthTableModel::WavelengthTableModel(QObject* parent) :
    QAbstractTableModel(parent)
{

}

int WavelengthTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

int WavelengthTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_wvls.size();
}

Qt::ItemFlags WavelengthTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return ( QAbstractItemModel::flags(index) | Qt::ItemIsEditable );
}


QVariant WavelengthTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::EditRole || role == Qt::DisplayRole){
        switch (j) {
        case static_cast<int>(WavelengthTableColumn::Value):
            return m_wvls[i].value;
        case static_cast<int>(WavelengthTableColumn::Weight):
            return m_wvls[i].weight;
        //case static_cast<int>(WavelengthTableColumn::Color):
        //    return m_wvls[i].color;
        default:
            return QVariant();
        }
    }else if(role == Qt::BackgroundRole || role == Qt::DecorationRole){
        if(j == static_cast<int>(WavelengthTableColumn::Color)){
            return m_wvls[i].color;
        }
    }

    return QVariant();
}

QVariant WavelengthTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    if(orientation == Qt::Horizontal){
        switch (section) {
        case static_cast<int>(WavelengthTableColumn::Value):
            return tr("Value");
        case static_cast<int>(WavelengthTableColumn::Weight):
            return tr("Weight");
        case static_cast<int>(WavelengthTableColumn::Color):
            return tr("Color");
        default:
            return QVariant();
        }
    }else{ // Vertical
        return QVariant(section);
    }

}

bool WavelengthTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) return false;

    const int i = index.row();
    const int j = index.column();

    if(role == Qt::EditRole){

        if(j == static_cast<int>(WavelengthTableColumn::Value)){
            m_wvls[i].value = value.toDouble();
        }else if(j == static_cast<int>(WavelengthTableColumn::Weight)){
            m_wvls[i].weight = value.toDouble();
        }else if(j == static_cast<int>(WavelengthTableColumn::Color)){
            m_wvls[i].color = value.value<QColor>();
        }

        emit dataChanged(index, index);
        return true;

    }else if(role == Qt::BackgroundRole){
        m_wvls[i].color = value.value<QColor>();
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void WavelengthTableModel::setData(const std::shared_ptr<OpticalSystem> opt_sys)
{
    m_wvls.clear();

    WavelengthData wvl_data;
    const int num_wvls = opt_sys->optical_spec()->spectral_region()->wvl_count();

    beginInsertRows(QModelIndex(), 0, num_wvls-1);

    for(int wi = 0; wi < num_wvls; wi++){
        Wvl* wvl = opt_sys->optical_spec()->spectral_region()->wvl(wi);

        wvl_data.value = wvl->value();
        wvl_data.weight = wvl->weight();
        wvl_data.color = rgbToQColor(wvl->render_color());

        m_wvls.append(wvl_data);
    }

    endInsertRows();
    emit dataChanged(this->index(0,0), this->index(m_wvls.size()-1, 2));
}

bool WavelengthTableModel::insertRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginInsertRows(QModelIndex(), row, row+count-1);
    for(int i = 0; i < count; i++){
        m_wvls.insert(row, WavelengthData());
    }
    endInsertRows();

    return true;
}

bool WavelengthTableModel::removeRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; i++){
        m_wvls.remove(row);
    }
    endRemoveRows();
    return true;
}

void WavelengthTableModel::addRow()
{
    beginInsertRows(QModelIndex(), m_wvls.count(), m_wvls.count());
    m_wvls.append(WavelengthData());
    endInsertRows();
}

void WavelengthTableModel::applyData(std::shared_ptr<OpticalSystem> opt_sys)
{
    opt_sys->optical_spec()->spectral_region()->clear();

    for(int wi = 0; wi < m_wvls.size(); wi++){
        double val = m_wvls[wi].value;
        double wt = m_wvls[wi].weight;
        Rgb color = QColorToRgb(m_wvls[wi].color);

        opt_sys->optical_spec()->spectral_region()->add(val, wt, color);
    }
}

Rgb WavelengthTableModel::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}

QColor WavelengthTableModel::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}
