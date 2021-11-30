#ifndef QOPTICALSPEC_H
#define QOPTICALSPEC_H

#include <QObject>
#include "spec/optical_spec.h"
using namespace geopter;

#include "QFieldSpec.h"
#include "QPupilSpec.h"


class QOpticalSpec : public QObject, public OpticalSpec
{
    Q_OBJECT

public:
    QOpticalSpec();

    QFieldSpec* field_spec() const;
    QPupilSpec* pupil_spec() const;

public Q_SLOTS:

};

#endif // QOPTICALSPEC_H
