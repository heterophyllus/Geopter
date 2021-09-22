#ifndef PRESCRIPTIONDOCK_H
#define PRESCRIPTIONDOCK_H

#include "text_view_dock.h"
#include "optical.h"
using namespace geopter;

class PrescriptionDock : public TextViewDock
{
    Q_OBJECT

public:
    PrescriptionDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~PrescriptionDock();

    void updateText() override;

private:
    OpticalSystem* m_opticalSystem;

};

#endif // PRESCRIPTIONDOCK_H
