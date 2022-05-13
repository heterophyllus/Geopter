#ifndef LENS_DATA_CONSTANT_H
#define LENS_DATA_CONSTANT_H

enum LensDataColumn
{
    Label, // editable
    SurfaceType,
    Radius, // editable
    RadiusSolve,
    Thickness, // editable
    ThicknessSolve,
    Material, // editable
    Mode,
    SemiDiameter,
    Aperture,
};

enum class FieldTableColumn{
    X,
    Y,
    Weight,
    Color,
    VUY,
    VLY,
    VUX,
    VLX
};


enum class WavelengthTableColumn{
    Value,
    Weight,
    Color
};

#endif
