#ifndef PARAXIAL_TRACE_DIALOG_H
#define PARAXIAL_TRACE_DIALOG_H

#include <QDialog>

namespace Ui {
class ParaxialTraceDialog;
}

class ParaxialTraceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParaxialTraceDialog(QWidget *parent = nullptr);
    ~ParaxialTraceDialog();

private:
    Ui::ParaxialTraceDialog *ui;
};

#endif // PARAXIAL_TRACE_DIALOG_H
