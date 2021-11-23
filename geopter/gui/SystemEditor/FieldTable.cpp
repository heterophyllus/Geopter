#include "FieldTable.h"
#include "SystemEditor/FloatDelegate.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QColorDialog>
#include <QDebug>

FieldTable::FieldTable(QWidget* parent)
    : QTableWidget(parent),
      m_displayDigit(4)
{
    QStringList hHeaderLabels({"X", "Y", "Weight", "Color", "VUY", "VLY", "VUX", "VLX"});
    this->setColumnCount(hHeaderLabels.size());
    this->setHorizontalHeaderLabels(hHeaderLabels);
    this->setRowCount(1);
    this->postProcess();
    this->setupVerticalHeader();

    this->setItemDelegateForColumn(FieldTable::X,      new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::Y,      new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::Weight, new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::VUY,    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::VLY,    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::VUX,    new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(FieldTable::VLX,    new FloatDelegate(m_displayDigit, true, this));

    //this->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    //QObject::connect(this->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnHeader(const QPoint &)));

    QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onDoubleClick(QTableWidgetItem*)));

}

void FieldTable::setupVerticalHeader()
{
    int fieldCount = this->rowCount();
    QStringList vHeaderLabels;

    for( int fi = 0; fi < fieldCount; fi++){
        vHeaderLabels << QString::number(fi);
    }

    this->setVerticalHeaderLabels(vHeaderLabels);
}

void FieldTable::postProcess()
{
    for(int i = 0; i < this->rowCount(); i++) {
        for(int j = 0; j < this->columnCount(); j++) {
            QTableWidgetItem* item = this->item(i, j);
            if(!item){
                this->setItem(i, j, new QTableWidgetItem);
            }

            item = this->item(i, j);
            if(FieldTable::Color == j) {
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            }
        }
    }
}


void FieldTable::showContextMenuOnHeader(const QPoint &pos)
{
    qDebug() << "FieldTable::showContextMenu() in";

    QMenu contextMenu;
    contextMenu.clear();

    QAction *action1 = contextMenu.addAction("Insert");
    action1->setEnabled(true);

    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertField()));

    QAction *action2 = contextMenu.addAction("Remove");
    QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeField()));

    QAction *action3 = contextMenu.addAction("Add");
    QObject::connect(action3, SIGNAL(triggered()), this, SLOT(addField()));


    contextMenu.exec(QCursor::pos());

    qDebug() << "FieldTable::showContextMenu() out";
}

void FieldTable::insertField()
{
    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->insertRow(row);
    this->postProcess();

    int fi = row;
    Field* fld = new Field(); // create temporary field to get default value
    this->setFieldData(fi, fld);
    delete fld;

}

void FieldTable::removeField()
{
    Q_ASSERT(this->rowCount() > 0);

    if(this->rowCount() == 1){
        QMessageBox::warning(this,tr("Error"), tr("At least one field must be set"));
        return;
    }

    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->removeRow(row);
    this->postProcess();
}

void FieldTable::addField()
{
    int currentRowCount = this->rowCount();
    this->setRowCount(currentRowCount + 1);
    this->postProcess();

    Field* fld = new Field();
    int fi = this->rowCount()-1;
    this->setFieldData(fi, fld);
    delete fld;

    this->setupVerticalHeader();
}


void FieldTable::setFieldData(int row, const Field* fld)
{
    if(!fld) return;

    this->item(row, FieldTable::X)->setData(Qt::EditRole, fld->x());
    this->item(row, FieldTable::Y)->setData(Qt::EditRole, fld->y());
    this->item(row, FieldTable::Weight)->setData(Qt::EditRole, fld->weight());
    this->item(row, FieldTable::Color)->setBackground(QBrush( rgbToQColor(fld->render_color()) ));
    this->item(row, FieldTable::VUY)->setData(Qt::EditRole, fld->vuy());
    this->item(row, FieldTable::VLY)->setData(Qt::EditRole, fld->vly());
    this->item(row, FieldTable::VUX)->setData(Qt::EditRole, fld->vux());
    this->item(row, FieldTable::VLX)->setData(Qt::EditRole, fld->vlx());
}


void FieldTable::importFieldData(const std::shared_ptr<OpticalSystem> optsys)
{
    if( !optsys){
        return;
    }

    int fieldCount = optsys->optical_spec()->field_of_view()->field_count();

    if(this->rowCount() != fieldCount){
        this->setRowCount(fieldCount);
        this->postProcess();
    }

    for(int fi = 0; fi < fieldCount; fi++) {
        Field* fld = optsys->optical_spec()->field_of_view()->field(fi);
        this->setFieldData(fi, fld);
    }

}

void FieldTable::applyCurrentData(std::shared_ptr<OpticalSystem> optsys)
{
    optsys->optical_spec()->field_of_view()->clear();
    for(int fi = 0; fi < this->rowCount(); fi++){
        double x   = this->item(fi, FieldTable::X)->data(Qt::EditRole).toDouble();
        double y   = this->item(fi, FieldTable::Y)->data(Qt::EditRole).toDouble();
        double wt  = this->item(fi, FieldTable::Weight)->data(Qt::EditRole).toDouble();
        QColor color = this->item(fi, FieldTable::Color)->background().color();
        Rgb render_color = QColorToRgb(color);
        double vuy = this->item(fi, FieldTable::VUY)->data(Qt::EditRole).toDouble();
        double vly = this->item(fi, FieldTable::VLY)->data(Qt::EditRole).toDouble();
        double vux = this->item(fi, FieldTable::VUX)->data(Qt::EditRole).toDouble();
        double vlx = this->item(fi, FieldTable::VLX)->data(Qt::EditRole).toDouble();

        optsys->optical_spec()->field_of_view()->add(x, y, wt, render_color, vuy, vly, vux, vlx);
    }
}

void FieldTable::onDoubleClick(QTableWidgetItem* item)
{
    if(!item) return;

    int col = item->column();

    if(FieldTable::Color == col){
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            item->setBackground(QBrush(color));
        }
    }
}

QColor FieldTable::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}

Rgb FieldTable::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}
