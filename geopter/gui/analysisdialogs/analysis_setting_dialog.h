#ifndef ANALYSISSETTINGDIALOG_H
#define ANALYSISSETTINGDIALOG_H

#include <QDialog>

class AnalysisSettingDialog : public QDialog
{
    Q_OBJECT

public:
    AnalysisSettingDialog(QWidget *parent);

    virtual void updateParentDockContent();
};

#endif // ANALYSISSETTINGDIALOG_H
