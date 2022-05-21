#ifndef WAVELENGTH_TABLE_MODEL_H
#define WAVELENGTH_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "WavelengthData.h"
#include "optical.h"
using namespace geopter;

class WavelengthTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit WavelengthTableModel(QObject* parent = nullptr);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addRow();
    void setData(const std::shared_ptr<OpticalSystem> opt_sys);
    void applyData(std::shared_ptr<OpticalSystem> opt_sys);

private:
    QVector<WavelengthData> m_wvls;

    QColor rgbToQColor(const Rgb& rgb);
    Rgb QColorToRgb(const QColor& color);
};

#endif //WAVELENGTH_TABLE_MODEL_H

