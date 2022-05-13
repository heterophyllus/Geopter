#ifndef SURFACE_DATA_H
#define SURFACE_DATA_H

#include <QString>
#include <QVariant>

struct SurfaceData
{
    SurfaceData();
    SurfaceData(QString lbl, QString typ, double r, double t, QString mat, QString m, double sd, QString ape);

    void setData(QString lbl, QString typ, double r, double t, QString mat, QString m, double sd, QString ape);
    void setData(int column, const QVariant &value);

    QVariant data(int column);

    QString label;
    QString type;
    double radius;
    double thickness;
    QString material;
    QString mode;
    double semi_diameter;
    QString aperture;

};

#endif //SURFACE_DATA_H
