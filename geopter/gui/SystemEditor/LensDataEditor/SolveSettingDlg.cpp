#include "SolveSettingDlg.h"
#include "ui_SolveSettingDlg.h"

#include "optical.h"
using namespace geopter;

SolveSettingDlg::SolveSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolveSettingDlg)
{
    ui->setupUi(this);

    QStringList items({"Fixed", "Edge Thickness", "Overall Length", "Marginal Height"});
    ui->comboSolveType->addItems(items);

    QObject::connect(ui->comboSolveType, SIGNAL(currentIndexChanged(int)), this, SLOT(SetUiForSolve(int)));

}

SolveSettingDlg::~SolveSettingDlg()
{
    delete ui;
}

void SolveSettingDlg::GetParameters(int *solvetype, double *param1, double *param2, double *param3, double *param4)
{
    *solvetype = ui->comboSolveType->currentIndex();
    *param1 = ui->lineEditParam1->text().toDouble();
    *param2 = ui->lineEditParam2->text().toDouble();
    *param3 = ui->lineEditParam3->text().toDouble();
    *param4 = ui->lineEditParam4->text().toDouble();
}

void SolveSettingDlg::SetParameters(int solvetype, double param1, double param2, double param3, double param4)
{
    SetUiForSolve(solvetype);
    ui->lineEditParam1->setText(QString::number(param1));
    ui->lineEditParam2->setText(QString::number(param2));
    ui->lineEditParam3->setText(QString::number(param3));
    ui->lineEditParam4->setText(QString::number(param4));
}

void SolveSettingDlg::SetUiForSolve(int solve_type)
{
    ui->comboSolveType->setCurrentIndex(solve_type);

    switch (solve_type) {
    case Solve::SolveType::Fixed:
        SetParameterUiVisible(false, false, false, false);
        break;
    case Solve::SolveType::EdgeThickness:
        SetParameterUiVisible(true, true, false, false);
        ui->labelParam1->setText("Thickness");
        ui->labelParam2->setText("Height");
        break;
    case Solve::SolveType::MarginalHeight:
        SetParameterUiVisible(true, true, false, false);
        ui->labelParam1->setText("Height");
        ui->labelParam2->setText("Pupil Zone");
        break;
    default:
        SetParameterUiVisible(false, false, false, false);
        break;
    }

    this->update();
}


void SolveSettingDlg::SetParameterUiVisible(bool state1, bool state2, bool state3, bool state4)
{
    ui->labelParam1->setVisible(state1);
    ui->lineEditParam1->setVisible(state1);

    ui->labelParam2->setVisible(state2);
    ui->lineEditParam2->setVisible(state2);

    ui->labelParam3->setVisible(state3);
    ui->lineEditParam3->setVisible(state3);

    ui->labelParam4->setVisible(state4);
    ui->lineEditParam4->setVisible(state4);

    this->update();
}

