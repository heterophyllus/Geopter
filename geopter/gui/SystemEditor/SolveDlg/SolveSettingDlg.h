#ifndef SOLVESETTINGDLG_H
#define SOLVESETTINGDLG_H

#include <QDialog>
#include <QList>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class SolveSettingDlg;
}

class SolveSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SolveSettingDlg(QWidget *parent = nullptr);
    ~SolveSettingDlg();

    void GetParameters(double *param1, double *param2, double *param3, double *param4) const;
    int GetSolveType() const;
    void SetCurrentSolve(int solvetype, double param1, double param2, double param3, double param4);

public slots:
    void SetFormForSolveType(int solvetype);

private:
    void SetParametersVisible(bool param1state, bool param2state, bool param3state, bool param4state);
    void SetParameterLabel(int param, QString label);
    void SetParameterValue(int param_number, double value, bool is_integer= false);
    Ui::SolveSettingDlg *ui;
    QList<QLabel*> m_labelList;
    QList<QLineEdit*> m_editList;
};

#endif // SOLVESETTINGDLG_H
