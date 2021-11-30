#ifndef QOPTICALSYSTEM_H
#define QOPTICALSYSTEM_H

#include <QObject>
#include "system/optical_system.h"

#include "QOpticalSpec.h"

using namespace geopter;


/** OpticalSystem wrapper to be accessed from PythonQt.
 *
 *  This is actually OpticalSystem which inherits QObject.
 *  Q_SLOTS functions are used for scripting.
 */
class QOpticalSystem : public QObject, public OpticalSystem //Be sure that the first inherited class is a QObject
{
    Q_OBJECT

public:
    QOpticalSystem();
    ~QOpticalSystem();


public Q_SLOTS:
    QString title() const;
    QString note() const;

    double entrance_pupil_radius(){
        fund_data_.enp_radius;
    }



    //void set_title(const QString& title);
    //void set_note(const QString& note);

};

#endif // QOPTICALSYSTEM_H
