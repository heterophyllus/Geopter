#ifndef QLENSDATAEDITOR_H
#define QLENSDATAEDITOR_H

#include <QObject>

class QOpticalSystem;

class QLensDataEditor : public QObject
{
    Q_OBJECT

public:
    QLensDataEditor(QOpticalSystem* opt_sys);
    ~QLensDataEditor();

public Q_SLOTS:
    double GetRadiusAt(int i) const;
    double GetThicknessAt(int i) const;
    QString GetMaterialAt(int i) const;
    QString GetLabelAt(int i) const;

    void InsertNewSurfaceAt(int i);
    void SetSurfaceDataAt(int i, double radius, double thick, QString material, QString comment);
    void SetRadiusAt(int i, double r);
    void SetThicknessAt(int i, double t);
    void SetMaterialAt(int i, QString material);

private:
    QOpticalSystem* parent_;
};

#endif // QLENSDATAEDITOR_H
