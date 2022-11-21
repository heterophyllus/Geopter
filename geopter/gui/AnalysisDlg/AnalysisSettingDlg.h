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
    AnalysisSettingDlg(OpticalSystem* sys, QWidget *parent) : QDialog(parent), m_opticalSystem(sys){}

    virtual void updateParentDockContent() = 0;
    virtual int parentDockType() const{
        return 0;
    }

protected:
    OpticalSystem *m_opticalSystem;
    QMap<QString, QVariant> m_defaultParams;
};

#endif // AnalysisSettingDlg_H
