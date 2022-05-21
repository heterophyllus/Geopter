#ifndef GEOPTER_FIELD_DATA_H
#define GEOPTER_FIELD_DATA_H

#include <QColor>

struct FieldData
{
    FieldData(){
        x = 0.0;
        y = 0.0;
        wt = 1.0;
        vux = 0.0;
        vuy = 0.0;
        vlx = 0.0;
        vly = 0.0;
        color = QColor(255,255,255);
    }

    double x;
    double y;
    double wt;
    double vux;
    double vuy;
    double vlx;
    double vly;
    QColor color;
};


#endif //GEOPTER_FIELD_DATA_H
