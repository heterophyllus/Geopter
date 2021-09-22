#ifndef SINGLERAYTRACEDOCK_H
#define SINGLERAYTRACEDOCK_H

#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;

class SingleRayTraceDock : public TextViewDock
{
    Q_OBJECT

public:
    SingleRayTraceDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~SingleRayTraceDock();

    void updateText() override;

private:
    void doPupilRayTrace();
    void doObjectRayTrace();

    OpticalSystem* m_opticalSystem;
};

#endif // SINGLERAYTRACEDOCK_H
