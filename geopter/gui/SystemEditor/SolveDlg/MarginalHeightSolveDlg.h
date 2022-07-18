#ifndef MARGINAL_HEIGHT_SOLVE_DLG_H
#define MARGINAL_HEIGHT_SOLVE_DLG_H

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QValidator>
#include <QDialogButtonBox>

class MarginalHeightSolveDlg : public QDialog
{
    Q_OBJECT

public:
    MarginalHeightSolveDlg(QWidget* parent = nullptr) : QDialog(parent){
        //setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Marginal Height Solve");

        QLabel *valueLabel = new QLabel("Value");
        QLabel *zoneLabel = new QLabel("Zone");
        m_valueEdit = new QLineEdit;
        m_valueEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));
        m_zoneEdit = new QLineEdit;
        m_zoneEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));

        m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(valueLabel,   0, 0);
        layout->addWidget(zoneLabel,  1, 0);
        layout->addWidget(m_valueEdit,  0, 1);
        layout->addWidget(m_zoneEdit, 1, 1);
        layout->addWidget(m_buttonBox,  2, 0, 1, 2);

        this->setLayout(layout);
    };

    double value(){
        return m_valueEdit->text().toDouble();
    };
    double zone(){
        return m_zoneEdit->text().toDouble();
    };


private:
    QLineEdit* m_zoneEdit;
    QLineEdit* m_valueEdit;
    QDialogButtonBox* m_buttonBox;
};

#endif //EDGE_THIVKNESS_SOLVE_DLG_H
