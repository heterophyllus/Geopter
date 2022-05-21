#ifndef WAVELENGTH_DATA_H
#define WAVELENGTH_DATA_H

#include <QColor>

struct WavelengthData
{
    WavelengthData(){
        value = 587.562;
        weight = 1.0;
        color = Qt::black;
    }

    double value; // nm
    double weight;
    QColor color;
};


#endif //WAVELENGTH_DATA_H

