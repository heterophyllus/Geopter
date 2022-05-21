#ifndef SPECEDITORDLG_H
#define SPECEDITORDLG_H

#include <QDialog>
#include "optical.h"
using namespace geopter;

namespace Ui {
class SpecEditorDlg;
}

class SpecEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SpecEditorDlg(QWidget *parent = nullptr);
    ~SpecEditorDlg();

    void loadData(const std::shared_ptr<OpticalSystem> optsys);
    void applyData(std::shared_ptr<OpticalSystem> optsys);

private slots:
    void catchValueEdited();
    void setupReferenceWavelengthCombo(int current= -1);

private:
    Ui::SpecEditorDlg *ui;

};

#endif // SPECEDITORDLG_H
