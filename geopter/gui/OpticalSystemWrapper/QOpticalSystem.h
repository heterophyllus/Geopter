#ifndef QOPTICALSYSTEM_H
#define QOPTICALSYSTEM_H

#include <QObject>
#include "system/optical_system.h"

#include "QLensDataEditor.h"

using namespace geopter;


/** OpticalSystem wrapper to be accessed from PythonQt.
 *
 *  Q_SLOTS functions are used for scripting.
 */
class QOpticalSystem : public QObject, public OpticalSystem //Be sure that the first inherited class is a QObject
{
    Q_OBJECT

public:
    QOpticalSystem();
    ~QOpticalSystem();

public Q_SLOTS:

    QString GetTitle() const;
    QString GetNote() const;
    double EntrancePupilDiameter() const;
    double EntrancePupilDistance() const;
    double ExitPupilDiameter() const;
    double ExitPupilDistance() const;
    int NumberOfSurfaces() const;
    int NumberOfFields() const;
    int NumberOfWavelengths() const;
    double FocalLength(int start, int end) const;
    double FocalLength() const; // overall

    QLensDataEditor* LensDataEditor() const;

    void SetTitle(QString note);
    void SetNote(QString note);

protected:
    /** Output to python console */
    void PythonStdOut(const QString& text) const;

    /** Check the given suraface index */
    bool CheckSurfaceIndex(int si) const;

    std::unique_ptr<QLensDataEditor> lde_;

};

#endif // QOPTICALSYSTEM_H
