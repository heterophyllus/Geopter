#ifndef LENS_DATA_MODEL_H
#define LENS_DATA_MODEL_H

#include <QAbstractTableModel>
#include "OpticalSystemWrapper/QOpticalSystem.h"

class LensDataTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LensDataTableModel(std::shared_ptr<OpticalSystem> opt_sys, QObject *parent = nullptr);
    virtual ~LensDataTableModel() override;

    void setOpticalSystem(std::shared_ptr<OpticalSystem> opt_sys);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void setStop(int i);

private:
    std::shared_ptr<OpticalSystem> m_opt_sys;
};

#endif
