#include "WavelengthTable.h"
#include "SystemEditor/FloatDelegate.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QColorDialog>
#include <QInputDialog>
#include <QDebug>

WavelengthTable::WavelengthTable(QWidget* parent)
    : QTableWidget(parent),
      m_displayDigit(4)
{
    QStringList hHeaderLabels({"Value", "Weight", "Color"});
    this->setColumnCount(hHeaderLabels.size());
    this->setHorizontalHeaderLabels(hHeaderLabels);
    this->setRowCount(1);
    this->postProcess();
    this->setupVerticalHeader();

    this->setItemDelegateForColumn(WavelengthTable::Value,  new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(WavelengthTable::Weight, new FloatDelegate(m_displayDigit, true, this));

    this->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu()));

    QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onDoubleClick(QTableWidgetItem*)));

}


void WavelengthTable::setupVerticalHeader()
{
    int fieldCount = this->rowCount();
    QStringList vHeaderLabels;

    for( int fi = 0; fi < fieldCount; fi++){
        vHeaderLabels << QString::number(fi);
    }

    this->setVerticalHeaderLabels(vHeaderLabels);
}

void WavelengthTable::postProcess()
{
    for(int i = 0; i < this->rowCount(); i++) {
        for(int j = 0; j < this->columnCount(); j++) {
            QTableWidgetItem* item = this->item(i, j);
            if(!item){
                this->setItem(i, j, new QTableWidgetItem);
            }

            item = this->item(i, j);
            if(WavelengthTable::Color == j) {
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            }
        }
    }
}

void WavelengthTable::showContextMenu()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Insert");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertWavelength()));

    QAction *action2 = contextMenu.addAction("Remove");
    QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeWavelength()));

    QAction *action3 = contextMenu.addAction("Add");
    QObject::connect(action3, SIGNAL(triggered()), this, SLOT(addWavelength()));


    contextMenu.exec(QCursor::pos());
}

void WavelengthTable::insertWavelength()
{
    int row = this->currentRow();

    if(row < 0 || row >= this->rowCount()) return;

    this->insertRow(row);
    this->postProcess();

    int wi = row;
    Wvl* wvl = new Wvl();
    this->setWavelengthData(wi, wvl);
    delete wvl;
}

void WavelengthTable::removeWavelength()
{
    Q_ASSERT(this->rowCount() > 0);

    if(this->rowCount() == 1){
        QMessageBox::warning(this,tr("Error"), tr("At least one wavelength must be set"));
        return;
    }

    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->removeRow(row);
    this->postProcess();
}

void WavelengthTable::addWavelength()
{
    int currentRowCount = this->rowCount();
    this->setRowCount(currentRowCount + 1);
    this->postProcess();

    int wi = this->rowCount() - 1;
    Wvl* wvl = new Wvl();
    this->setWavelengthData(wi, wvl);
    delete wvl;
}

void WavelengthTable::setWavelengthData(int row, const Wvl* wvl)
{
    this->item(row, WavelengthTable::Value)->setData(Qt::EditRole, wvl->value());
    this->item(row, WavelengthTable::Weight)->setData(Qt::EditRole, wvl->weight());
    this->item(row, WavelengthTable::Color)->setBackground(QBrush(rgbToQColor(wvl->render_color())));
}


void WavelengthTable::importWavelengthData(const std::shared_ptr<OpticalSystem> optsys)
{
    if( !optsys){
        return;
    }

    int wvlCount = optsys->optical_spec()->spectral_region()->wvl_count();

    if(this->rowCount() != wvlCount){
        this->setRowCount(wvlCount);
        this->postProcess();
    }

    for(int wi = 0; wi < wvlCount; wi++) {
        Wvl* wvl = optsys->optical_spec()->spectral_region()->wvl(wi);
        this->setWavelengthData(wi, wvl);
    }
}

void WavelengthTable::applyCurrentData(std::shared_ptr<OpticalSystem> optsys)
{
    optsys->optical_spec()->spectral_region()->clear();
    for(int wi = 0; wi < this->rowCount(); wi++) {
        double val = this->item(wi, WavelengthTable::Value)->data(Qt::EditRole).toDouble();
        double wt  = this->item(wi, WavelengthTable::Weight)->data(Qt::EditRole).toDouble();
        QColor qcolor = this->item(wi, WavelengthTable::Color)->background().color();
        Rgb color = QColorToRgb(qcolor);

        optsys->optical_spec()->spectral_region()->add(val, wt, color);
    }
}

void WavelengthTable::onDoubleClick(QTableWidgetItem* item)
{
    if(!item) return;

    int col = item->column();

    if(WavelengthTable::Color == col) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            item->setBackground(QBrush(color));
        }
    }
    else if(WavelengthTable::Value == col) {
        QStringList presets;
        presets << "t" << "s" << "r" << "C" << "C_" << "D" << "d" << "e" << "F" << "F_" << "g" << "h" << "i";

        bool ok = false;
        QString selected = QInputDialog::getItem(this, tr("Input Wavelength"), tr("Select spectral line or input value directly."), presets, 0,true, &ok);

        if(ok){
            if(presets.contains(selected)) {
                double val = SpectralLine::wavelength(selected.toStdString());
                item->setData(Qt::EditRole, val);
            }else{
                bool isNumber = false;
                double val = selected.toDouble(&isNumber);
                if(isNumber){
                    item->setData(Qt::EditRole, val);
                }
            }
        }
    }
}


QColor WavelengthTable::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}

Rgb WavelengthTable::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}

