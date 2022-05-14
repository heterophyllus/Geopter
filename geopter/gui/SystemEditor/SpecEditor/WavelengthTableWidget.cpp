#include "WavelengthTableWidget.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QColorDialog>
#include <QInputDialog>
#include <QDebug>
#include "SystemEditor/SystemDataConstant.h"

WavelengthTableWidget::WavelengthTableWidget(QWidget* parent)
    : QTableWidget(parent),
      m_settingUp(false),
      m_displayDigit(4)
{
    QStringList hHeaderLabels({"Value", "Weight", "Color"});
    this->setColumnCount(hHeaderLabels.size());
    this->setHorizontalHeaderLabels(hHeaderLabels);
    this->setRowCount(1);
    this->setupItems();
    this->setupVerticalHeader();

    this->setItemDelegateForColumn(static_cast<int>(WavelengthTableColumn::Value),  new FloatDelegate(m_displayDigit, true, this));
    this->setItemDelegateForColumn(static_cast<int>(WavelengthTableColumn::Weight), new FloatDelegate(m_displayDigit, true, this));

    QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onDoubleClick(QTableWidgetItem*)));
    QObject::connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),       this, SLOT(onItemChanged(QTableWidgetItem*)));
}


void WavelengthTableWidget::setupVerticalHeader()
{
    int fieldCount = this->rowCount();
    QStringList vHeaderLabels;

    for( int fi = 0; fi < fieldCount; fi++){
        vHeaderLabels << QString::number(fi);
    }

    this->setVerticalHeaderLabels(vHeaderLabels);
}

void WavelengthTableWidget::setupItems()
{
    for(int i = 0; i < this->rowCount(); i++) {
        for(int j = 0; j < this->columnCount(); j++) {
            QTableWidgetItem* item = this->item(i, j);
            if(!item){
                this->setItem(i, j, new QTableWidgetItem);
            }

            item = this->item(i, j);
            if( static_cast<int>(WavelengthTableColumn::Color) == j) {
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            }
        }
    }
}


void WavelengthTableWidget::insertWavelength()
{
    m_settingUp = true;

    int row = this->currentRow();

    if(row < 0 || row >= this->rowCount()) return;

    this->insertRow(row);
    this->setupItems();
    this->setupVerticalHeader();

    int wi = row;
    Wvl* wvl = new Wvl(); // get default values
    this->setWavelengthData(wi, wvl);
    delete wvl;

    m_settingUp = false;

    emit setupCompleted();
}

void WavelengthTableWidget::removeWavelength()
{
    Q_ASSERT(this->rowCount() > 0);

    m_settingUp = true;

    if(this->rowCount() == 1){
        QMessageBox::warning(this,tr("Error"), tr("At least one wavelength must be set"));
        return;
    }

    int row = this->currentRow();
    if(row < 0 || row >= this->rowCount()) return;

    this->removeRow(row);
    this->setupItems();
    this->setupVerticalHeader();

    m_settingUp = false;

    emit setupCompleted();
}

void WavelengthTableWidget::addWavelength()
{
    m_settingUp = true;

    int currentRowCount = this->rowCount();
    this->setRowCount(currentRowCount + 1);
    this->setupVerticalHeader();
    this->setupItems();

    int wi = this->rowCount() - 1;
    Wvl* wvl = new Wvl(); // get default values
    this->setWavelengthData(wi, wvl);
    delete wvl;

    m_settingUp = false;

    emit setupCompleted();
}

void WavelengthTableWidget::setWavelengthData(int row, const Wvl* wvl)
{
    this->item(row, static_cast<int>(WavelengthTableColumn::Value))->setData(Qt::EditRole, wvl->value());
    this->item(row, static_cast<int>(WavelengthTableColumn::Weight))->setData(Qt::EditRole, wvl->weight());
    this->item(row, static_cast<int>(WavelengthTableColumn::Color))->setBackground(QBrush(rgbToQColor(wvl->render_color())));
}


void WavelengthTableWidget::importWavelengthData(const std::shared_ptr<OpticalSystem> optsys)
{
    if( !optsys){
        return;
    }

    m_settingUp = true;

    int wvlCount = optsys->optical_spec()->spectral_region()->wvl_count();

    if(this->rowCount() != wvlCount){
        this->setRowCount(wvlCount);
        this->setupItems();
    }

    for(int wi = 0; wi < wvlCount; wi++) {
        Wvl* wvl = optsys->optical_spec()->spectral_region()->wvl(wi);
        this->setWavelengthData(wi, wvl);
    }

    m_settingUp = false;
}

void WavelengthTableWidget::applyCurrentData(std::shared_ptr<OpticalSystem> optsys)
{
    optsys->optical_spec()->spectral_region()->clear();
    for(int wi = 0; wi < this->rowCount(); wi++) {
        double val = this->item(wi, static_cast<int>(WavelengthTableColumn::Value))->data(Qt::EditRole).toDouble();
        double wt  = this->item(wi, static_cast<int>(WavelengthTableColumn::Weight))->data(Qt::EditRole).toDouble();
        QColor qcolor = this->item(wi, static_cast<int>(WavelengthTableColumn::Color))->background().color();
        Rgb color = QColorToRgb(qcolor);

        optsys->optical_spec()->spectral_region()->add(val, wt, color);
    }
}

void WavelengthTableWidget::onDoubleClick(QTableWidgetItem* item)
{
    if(!item) return;

    int col = item->column();

    if( static_cast<int>(WavelengthTableColumn::Color) == col) {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Color"), QColorDialog::DontUseNativeDialog);
        if (color.isValid()) {
            item->setBackground(QBrush(color));
        }
    }
    else if( static_cast<int>(WavelengthTableColumn::Value) == col) {
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

        emit valueEdited();
    }
}

void WavelengthTableWidget::onItemChanged(QTableWidgetItem* item)
{
    if( !m_settingUp) {
        int col = item->column();

        if( static_cast<int>(WavelengthTableColumn::Value) == col ){
            emit valueEdited();
        }
    }
}

QColor WavelengthTableWidget::rgbToQColor(const Rgb& rgb)
{
    int r = (int)(255.0*rgb.r);
    int g = (int)(255.0*rgb.g);
    int b = (int)(255.0*rgb.b);
    //int a = (int)(255.0*rgb.a);
    int a = 255;

    return QColor(r,g,b,a);
}

Rgb WavelengthTableWidget::QColorToRgb(const QColor& color)
{
    double r = (double)color.red() / 255.0;
    double g = (double)color.green() / 255.0;
    double b = (double)color.blue() / 255.0;
    double a = (double)color.alpha() / 255.0;

    return Rgb(r,g,b,a);
}

