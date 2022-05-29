#include "WavelengthTableView.h"
#include "SystemEditor/SystemDataConstant.h"
#include "SystemEditor/Delegate/NotEditableDelegate.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include <QColorDialog>
#include <QInputDialog>

WavelengthTableView::WavelengthTableView(QWidget* parent) :
    QTableView(parent)
{
    m_digit = 4;
    m_model = new WavelengthTableModel();
    this->setModel(m_model);

    this->setItemDelegateForColumn(static_cast<int>(WavelengthTableColumn::Value),  new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(WavelengthTableColumn::Weight), new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(WavelengthTableColumn::Color),  new NotEditableDelegate(this));

    QObject::connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClick(const QModelIndex &)));
    QObject::connect(this->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)), this, SLOT(emitValueEdited()));
}

void WavelengthTableView::insertRow()
{
    const int row = this->selectedIndexes().at(0).row();
    dynamic_cast<WavelengthTableModel*>(this->model())->insertRows(row, 1);
    emit valueEdited();
}

void WavelengthTableView::removeRow()
{
    const int row = this->selectedIndexes().at(0).row();
    dynamic_cast<WavelengthTableModel*>(this->model())->removeRows(row, 1);
    emit valueEdited();
}

void WavelengthTableView::addRow()
{
    dynamic_cast<WavelengthTableModel*>(this->model())->addRow();
    emit valueEdited();
}

void WavelengthTableView::onDoubleClick(const QModelIndex& index)
{
    const int column = index.column();
    if(column == static_cast<int>(WavelengthTableColumn::Value)){
        // create item list
        QStringList spectralNames({"t", "s", "r", "C", "C\'", "D", "d", "e", "F", "g", "h", "i"});
        const int num_spectrals = spectralNames.size();

        QStringList items;
        items.reserve(num_spectrals);
        for(int i = 0; i < num_spectrals; i++){
            items.append(spectralNames[i] + ": " + QString::number(SpectralLine::wavelength(spectralNames[i].toStdString())) + "nm" );
        }

        bool ok;
        QString item = QInputDialog::getItem(this, tr("Select Wavelength"), tr("You can select from preset spectrals or input value directly"), items, 0, true, &ok);
        if(ok){
            double value = 0.0;
            if(items.contains(item)){
                int select = items.indexOf(item);
                value = SpectralLine::wavelength(spectralNames[select].toStdString());
            }else{
                value = item.toDouble();
            }
            dynamic_cast<WavelengthTableModel*>(this->model())->setData(index, value, Qt::EditRole);
        }

    }else if(column == static_cast<int>(WavelengthTableColumn::Color)){
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if(color.isValid()){
            dynamic_cast<WavelengthTableModel*>(this->model())->setData(index, color, Qt::BackgroundRole);
        }
    }
}

void WavelengthTableView::emitValueEdited()
{
    emit valueEdited();
}
