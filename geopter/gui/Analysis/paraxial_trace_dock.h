#ifndef PARAXIALTRACEDOCK_H
#define PARAXIALTRACEDOCK_H

#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;

class ParaxialTraceDock : public TextViewDock
{
public:
    ParaxialTraceDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~ParaxialTraceDock();

    void updateText() override;

private:
    OpticalSystem *m_opticalSystem;
};

#endif // PARAXIALTRACEDOCK_H
