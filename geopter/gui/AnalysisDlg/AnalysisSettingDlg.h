#ifndef AnalysisSettingDlg_H
#define AnalysisSettingDlg_H

#include <QDialog>
#include <QMap>
#include <QVariant>

#include "optical.h"
using namespace geopter;

class AnalysisSettingDlg : public QDialog
{
    Q_OBJECT

public:
    AnalysisSettingDlg(OpticalSystem* sys, QWidget *parent);

    virtual void updateParentDockContent();
    virtual int parentDockType() const;

protected:
    OpticalSystem *m_opticalSystem;
    QMap<QString, QVariant> m_defaultParams;
};

#endif // AnalysisSettingDlg_H
