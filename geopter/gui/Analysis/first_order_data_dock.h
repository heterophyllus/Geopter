#ifndef FIRSTORDERDATADOCK_H
#define FIRSTORDERDATADOCK_H

#include "text_view_dock.h"

#include "optical.h"
using namespace geopter;

class FirstOrderDataDock : public TextViewDock
{
    Q_OBJECT

public:
    FirstOrderDataDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~FirstOrderDataDock();

    void updateText() override;

private:
    OpticalSystem* m_opticalSystem;
};

#endif // FIRSTORDERDATADOCK_H
