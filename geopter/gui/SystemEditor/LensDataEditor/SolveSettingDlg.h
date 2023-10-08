#ifndef SOLVESETTINGDLG_H
#define SOLVESETTINGDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QList>

namespace Ui {
class SolveSettingDlg;
}



class SolveSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SolveSettingDlg(QWidget *parent = nullptr);
    ~SolveSettingDlg();

    void GetParameters(int *solvetype, double *param1=nullptr, double *param2=nullptr, double *param3=nullptr, double *param4=nullptr);
    void SetParameters(int solvetype, double param1=0.0, double param2=0.0, double param3=0.0, double param4=0.0);

private slots:
    void SetUiForSolve(int solve_type);

private:
    Ui::SolveSettingDlg *ui;

    void SetParameterUiVisible(bool state1, bool state2, bool state3, bool state4);
};

#endif // SOLVESETTINGDLG_H
