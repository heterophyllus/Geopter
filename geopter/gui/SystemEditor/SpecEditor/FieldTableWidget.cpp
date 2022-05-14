#include "FieldTableWidget.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include "SystemEditor/SystemDataConstant.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QColorDialog>
#include <QDebug>


FieldTableWidget::FieldTableWidget(QWidget* parent)
    : QTableWidget(parent),
      m_displayDigit(4)
{
    QStringList hHeaderLabels({"X", "Y", "Weight", "Color", "VUY", "VLY", "VUX", "VLX"});
    this->setColumnCount(hHeaderLabels.size());
    this->setHorizontalHeaderLabels(hHeaderLabels);
    this->setRowCount(1);
    this->setupItems();
    this->setupVerticalHeader();

    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::X),      new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Y),      new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::Weight), new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUY),    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLY),    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VUX),    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(FieldTableColumn::VLX),    new FloatDelegate(m_displayDigit, true, this));

    QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onDoubleClick(QTableWidgetItem*)));

}

void FieldTableWidget::setupVerticalHeader()
{
    int fieldCount = this->rowCount();
    QStringList vHeaderLabels;

    for( int fi = 0; fi < fieldCount; fi++){
        vHeaderLabels << QString::number(fi);
    }

    this->setVerticalHeaderLabels(vHeaderLabels);
}

void FieldTableWidget::setupItems()
{
    for(int i = 0; i < this->rowCount(); i++) {
        for(int j = 0; j < this->columnCount(); j++) {
            QTableWidgetItem* item = this->item(i, j);
            if(!item){
                this->setItem(i, j, new QTableWidgetItem);
            }

            item = this->item(i, j);
            if(static_cast<int>(FieldTableColumn::Color) == j) {
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            }
        }
    }
}


void FieldTableWidget::insertField()
{
    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->insertRow(row);
    this->setupItems();
    this->setupVerticalHeader();

    int fi = row;
    Field* fld = new Field(); // create temporary field to get default value
    this->setFieldData(fi, fld);
    delete fld;

}

void FieldTableWidget::removeField()
{
    Q_ASSERT(this->rowCount() > 0);

    if(this->rowCount() == 1){
        QMessageBox::warning(this,tr("Error"), tr("At least one field must be set"));
        return;
    }

    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->removeRow(row);
    this->setupItems();
    this->setupVerticalHeader();
}

void FieldTableWidget::addField()
{
    int currentRowCount = this->rowCount();
    this->setRowCount(currentRowCount + 1);
    this->setupItems();

    Field* fld = new Field();
    int fi = this->rowCount()-1;
    this->setFieldData(fi, fld);
    delete fld;

    this->setupVerticalHeader();
}


void FieldTableWidget::setFieldData(int row, const Field* fld)
{
    if(!fld) return;

    this->item(row, static_cast<int>(FieldTableColumn::X))->setData(Qt::EditRole, fld->x());
    this->item(row, static_cast<int>(FieldTableColumn::Y))->setData(Qt::EditRole, fld->y());
    this->item(row, static_cast<int>(FieldTableColumn::Weight))->setData(Qt::EditRole, fld->weight());
    this->item(row, static_cast<int>(FieldTableColumn::Color))->setBackground(QBrush( rgbToQColor(fld->render_color()) ));
    this->item(row, static_cast<int>(FieldTableColumn::VUY))->setData(Qt::EditRole, fld->vuy());
    this->item(row, static_cast<int>(FieldTableColumn::VLY))->setData(Qt::EditRole, fld->vly());
    this->item(row, static_cast<int>(FieldTableColumn::VUX))->setData(Qt::EditRole, fld->vux());
    this->item(row, static_cast<int>(FieldTableColumn::VLX))->setData(Qt::EditRole, fld->vlx());
}


void FieldTableWidget::importFieldData(const std::shared_ptr<OpticalSystem> optsys)
{
    if( !optsys){
        return;
    }

    int fieldCount = optsys->optical_spec()->field_of_view()->field_count();

    if(this->rowCount() != fieldCount){
        this->setRowCount(fieldCount);
        this->setupItems();
        this->setupVerticalHeader();
    }

    for(int fi = 0; fi < fieldCount; fi++) {
        Field* fld = optsys->optical_spec()->field_of_view()->field(fi);
        this->setFieldData(fi, fld);
    }

}

void FieldTableWidget::applyCurrentData(std::shared_ptr<OpticalSystem> optsys)
{
    optsys->optical_spec()->field_of_view()->clear();
    for(int fi = 0; fi < this->rowCount(); fi++){
        double x   = this->item(fi, static_cast<int>(FieldTableColumn::X))->data(Qt::EditRole).toDouble();
        double y   = this->item(fi, static_cast<int>(FieldTableColumn::Y))->data(Qt::EditRole).toDouble();
        double wt  = this->item(fi, static_cast<int>(FieldTableColumn::Weight))->data(Qt::EditRole).toDouble();
        QColor color = this->item(fi, static_cast<int>(FieldTableColumn::Color))->background().color();
        Rgb render_color = QColorToRgb(color);
        double vuy = this->item(fi, static_cast<int>(FieldTableColumn::VUY))->data(Qt::EditRole).toDouble();
        double vly = this->item(fi, static_cast<int>(FieldTableColumn::VLY))->data(Qt::EditRole).toDouble();
        double vux = this->item(fi, static_cast<int>(FieldTableColumn::VUX))->data(Qt::EditRole).toDouble();
        double vlx = this->item(fi, static_cast<int>(FieldTableColumn::VLX))->data(Qt::EditRole).toDouble();

        optsys->optical_spec()->field_of_view()->add(x, y, wt, render_color, vuy, vly, vux, vlx);
    }
}

void FieldTableWidget::onDoubleClick(QTableWidgetItem* item)
{
    if(!item) return;

    int col = item->column();

    if(static_cast<int>(FieldTableColumn::Color) == col){
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            item->setBackground(QBrush(color));
        }
    }
}

QColor FieldTableWidget::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}

Rgb FieldTableWidget::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}
