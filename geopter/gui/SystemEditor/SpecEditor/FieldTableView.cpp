#include "FieldTableView.h"
#include "SystemEditor/SystemDataConstant.h"
#include "SystemEditor/Delegate/NotEditableDelegate.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include "SystemEditor/Delegate/OneByteDelegate.h"
#include <QColorDialog>


FieldTableView::FieldTableView(QWidget* parent) :
    QTableView(parent)
{
    m_digit = 4;

    m_model = new FieldTableModel(this);
    this->setModel(m_model);

    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::X),      new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Y),      new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Weight), new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLX),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLY),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUX),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUY),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Color),  new NotEditableDelegate(this));

    QObject::connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClick(const QModelIndex &)));
}

void FieldTableView::init()
{
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::X),      new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Y),      new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Weight), new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLX),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLY),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUX),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUY),    new FloatDelegate(m_digit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Color),  new NotEditableDelegate(this));
}

void FieldTableView::insertRow()
{
    const int row = this->selectedIndexes().at(0).row();
    dynamic_cast<FieldTableModel*>(this->model())->insertRows(row, 1);
}

void FieldTableView::removeRow()
{
    const int row = this->selectedIndexes().at(0).row();
    dynamic_cast<FieldTableModel*>(this->model())->removeRows(row, 1);
}

void FieldTableView::addRow()
{
    dynamic_cast<FieldTableModel*>(this->model())->addRow();
}

void FieldTableView::onDoubleClick(const QModelIndex& index)
{
    const int column = index.column();
    if(column == static_cast<int>(FieldTableColumn::Color)){
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if(color.isValid()){
            dynamic_cast<FieldTableModel*>(this->model())->setData(index, color, Qt::BackgroundRole);
        }
    }
}
