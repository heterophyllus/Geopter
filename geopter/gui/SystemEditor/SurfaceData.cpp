#include "SurfaceData.h"
#include "LensDataConstant.h"

SurfaceData::SurfaceData()
{
    label = "";
    type = "Standard";
    radius = 0.0;
    thickness = 0.0;
    material = "AIR";
    mode = "Transmit";
    semi_diameter = 0.0;
    aperture = "None";
}

SurfaceData::SurfaceData(QString lbl, QString typ, double r, double t, QString mat, QString m, double sd, QString ape)
{
    this->setData(lbl, typ, r, t, mat, m, sd, ape);
}

void SurfaceData::setData(QString lbl, QString typ, double r, double t, QString mat, QString m, double sd, QString ape)
{
    label = lbl;
    type = typ;
    radius = r;
    thickness = t;
    material = mat;
    mode = m;
    semi_diameter = sd;
    aperture = ape;
}

void SurfaceData::setData(int column, const QVariant &value)
{
    switch (column) {
    case LensDataColumn::Label:
        label = value.toString();
        break;
    case LensDataColumn::SurfaceType:
        type = value.toString();
        break;
    case LensDataColumn::Radius:
        radius = value.toDouble();
        break;
    case LensDataColumn::Thickness:
        thickness = value.toDouble();
        break;
    case LensDataColumn::Material:
        material = value.toString();
        break;
    case LensDataColumn::Mode:
        mode = value.toString();
        break;
    case LensDataColumn::SemiDiameter:
        semi_diameter = value.toDouble();
        break;
    case LensDataColumn::Aperture:
        aperture = value.toString();
        break;
    }
}

QVariant SurfaceData::data(int column)
{
    switch (column) {
    case LensDataColumn::Label:
        return label;
    case LensDataColumn::SurfaceType:
        return type;
    case LensDataColumn::Radius:
        return radius;
    case LensDataColumn::Thickness:
        return thickness;
    case LensDataColumn::Material:
        return material;
    case LensDataColumn::Mode:
        return mode;
    case LensDataColumn::SemiDiameter:
        return semi_diameter;
    case LensDataColumn::Aperture:
        return aperture;
    default:
        return QVariant();
    }
}
