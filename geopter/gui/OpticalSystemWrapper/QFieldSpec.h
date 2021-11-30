#ifndef QFIELDSPEC_H
#define QFIELDSPEC_H

#include <QObject>
#include "spec/field_spec.h"
using namespace geopter;

class QField;

class QFieldSpec : public QObject, public FieldSpec
{
    Q_OBJECT

public:
    QFieldSpec();

public Q_SLOTS:
    int number_of_fields() const;
    int get_field_type_int() const;
    void set_field_type() const;
    QString get_field_type_str() const;
    QField* field(int i) const;
};

#endif // QFIELDSPEC_H
