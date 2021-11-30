#include "QOpticalSystem.h"

QOpticalSystem::QOpticalSystem() : QObject(nullptr), OpticalSystem()
{
    //opt_spec_ = std::make_unique<QOpticalSpec>();
}

QOpticalSystem::~QOpticalSystem()
{

}

QString QOpticalSystem::title() const
{
    return QString().fromStdString(title_);
}

QString QOpticalSystem::note() const
{
    return QString().fromStdString(note_);
}
