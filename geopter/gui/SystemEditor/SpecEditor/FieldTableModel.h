#ifndef FIELD_TABLE_MODEL_H
#define FIELD_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "FieldData.h"
#include "optical.h"
using namespace geopter;

class FieldTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FieldTableModel(QObject* parent = nullptr);

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
    QVector<FieldData> m_fields;

    Rgb QColorToRgb(const QColor& color);
    QColor rgbToQColor(const Rgb& rgb);
};

#endif //FIELD_TABLE_MODEL_H
