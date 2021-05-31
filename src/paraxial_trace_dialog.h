#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include <QDialog>

#include "optical_core.h"
using namespace geopter;

class TextViewDock;

namespace Ui {
class ParaxialTraceDialog;
}



class ParaxialTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParaxialTraceDialog(OpticalModel* opt_model, TextViewDock *parent = nullptr);
    ~ParaxialTraceDialog();

private slots:
    void onAccept();

private:
    Ui::ParaxialTraceDialog *ui;

    TextViewDock* parentDock_;
    OpticalModel* opt_model_;
};

#endif // PARAXIAL_TRACE_DIALOG_H
