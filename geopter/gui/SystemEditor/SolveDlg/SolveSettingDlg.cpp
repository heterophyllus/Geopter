#include "SolveSettingDlg.h"
#include "ui_SolveSettingDlg.h"
#include <QDebug>
#include "optical.h"
using namespace geopter;

SolveSettingDlg::SolveSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolveSettingDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Solve");

    QStringList items({"Fix", "Edge Thickness", "Marginal Height"});
    ui->solveTypeCombo->addItems(items);

    QObject::connect(ui->solveTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFormForSolveType(int)));

    m_labelList.clear();
    m_labelList << ui->param1Label << ui->param2Label << ui->param3Label << ui->param4Label;

    m_editList.clear();
    m_editList << ui->param1Edit << ui->param2Edit << ui->param3Edit << ui->param4Edit;

    SetFormForSolveType(0);
}

SolveSettingDlg::~SolveSettingDlg()
{
    delete ui;
}

void SolveSettingDlg::GetParameters(double *param1, double *param2, double *param3, double *param4) const
{
    switch (ui->solveTypeCombo->currentIndex()) {
    case Solve::SolveType::Fixed:
        param1 = nullptr;
        param2 = nullptr;
        param3 = nullptr;
        param4 = nullptr;
        break;
    case Solve::SolveType::MarginalHeight:
        *param1 = ui->param1Edit->text().toDouble();
        *param2 = ui->param2Edit->text().toDouble();
        param3 = nullptr;
        param4 = nullptr;
        break;
    case Solve::SolveType::EdgeThickness:
        *param1 = ui->param1Edit->text().toDouble();
        *param2 = ui->param2Edit->text().toDouble();
        param3 = nullptr;
        param4 = nullptr;
        break;
    default:
        param1 = nullptr;
        param2 = nullptr;
        param3 = nullptr;
        param4 = nullptr;
    }

}


int SolveSettingDlg::GetSolveType() const
{
    return ui->solveTypeCombo->currentIndex();
}

void SolveSettingDlg::SetCurrentSolve(int solvetype, double param1, double param2, double param3, double param4)
{
    ui->solveTypeCombo->setCurrentIndex(solvetype);
    SetFormForSolveType(solvetype);

    switch (solvetype) {
    case Solve::SolveType::MarginalHeight:
        SetParameterValue(1, param1);
        SetParameterValue(2, param2);
        break;
    case Solve::SolveType::EdgeThickness:
        SetParameterValue(1, param1);
        SetParameterValue(2, param2);
        break;
    }

    this->update();
}

void SolveSettingDlg::SetParameterValue(int param_number, double value, bool is_integer)
{
    QString sval;

    if(is_integer){
        sval = QString::number(value);
    }else{
        sval = QString::number(static_cast<int>(value));
    }

    m_editList[param_number-1]->setText(sval);

    this->update();
}

void SolveSettingDlg::SetFormForSolveType(int solvetype)
{
    if(solvetype == Solve::SolveType::Fixed){
        SetParametersVisible(false, false, false, false);
    }
    else if(solvetype == Solve::SolveType::MarginalHeight){
        SetParametersVisible(true, true, false, false);
        SetParameterLabel(1, "Zone");
        SetParameterLabel(2, "Height");
    }
    else if(solvetype == Solve::SolveType::EdgeThickness){
        SetParametersVisible(true, true, false, false);
        SetParameterLabel(1, "Thickness");
        SetParameterLabel(2, "Height");
    }
    this->update();
}

void SolveSettingDlg::SetParametersVisible(bool param1state, bool param2state, bool param3state, bool param4state)
{
    QList<bool> stateList;
    stateList << param1state << param2state << param3state << param4state;

    for(int i = 0; i < m_labelList.size(); i++){
        m_labelList[i]->setVisible(stateList[i]);
        m_editList[i]->setVisible(stateList[i]);
    }

}

void SolveSettingDlg::SetParameterLabel(int param, QString label)
{
    m_labelList[param-1]->setText(label);
}

