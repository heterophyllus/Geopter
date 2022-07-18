#ifndef EVEN_ASPHERE_COEF_TABLE_WIDGET_H
#define EVEN_ASPHERE_COEF_TABLE_WIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>

class EvenAsphereCoefTableWidget : public QTableWidget
{
public:
    EvenAsphereCoefTableWidget(QWidget* parent = nullptr) : QTableWidget(parent){
        QStringList hLabels = {"Value"};
        QStringList vLabels;
        vLabels << "R" << "k";
        for(int i = 1; i <= 10; i++){
            vLabels << ("A" + QString::number((i+1)*2) );
        }
        this->setColumnCount(1);
        this->setRowCount(vLabels.size());
        this->setVerticalHeaderLabels(vLabels);

        // set items
        for(int i = 0; i < this->rowCount(); i++){
            QTableWidgetItem *item = new QTableWidgetItem;
            this->setItem(i,0,item);
        }
    }

    void getCoefData(double *radius, double *conic, QVector<double>& coefs){
        *radius = this->item(0, 0)->text().toDouble();

    }


};

#endif

