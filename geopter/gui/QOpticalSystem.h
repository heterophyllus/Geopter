#ifndef QOPTICALSYSTEM_H
#define QOPTICALSYSTEM_H

#include <QObject>
#include "optical.h"
using namespace geopter;


/** OpticalSystem wrapper class
 *  This class inherits QObject to access from PythonQt.
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

    void set_title(const QString& title);
    void set_note(const QString& note);


};

#endif // QOPTICALSYSTEM_H
