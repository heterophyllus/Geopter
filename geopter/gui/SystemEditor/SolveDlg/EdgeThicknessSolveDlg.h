#ifndef EDGE_THIVKNESS_SOLVE_DLG_H
#define EDGE_THIVKNESS_SOLVE_DLG_H

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QValidator>
#include <QDialogButtonBox>

class EdgeThicknessSolveDlg : public QDialog
{
    Q_OBJECT

public:
    EdgeThicknessSolveDlg(QWidget* parent = nullptr) : QDialog(parent){
        //setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Edge Thickness Solve");

        QLabel *valueLabel = new QLabel("Value");
        QLabel *heightLabel = new QLabel("Height");
        m_valueEdit = new QLineEdit;
        m_valueEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));
        m_heightEdit = new QLineEdit;
        m_heightEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));

        m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(valueLabel,   0, 0);
        layout->addWidget(heightLabel,  1, 0);
        layout->addWidget(m_valueEdit,  0, 1);
        layout->addWidget(m_heightEdit, 1, 1);
        layout->addWidget(m_buttonBox,  2, 0, 1, 2);

        this->setLayout(layout);
    };

    double value(){
        return m_valueEdit->text().toDouble();
    };
    double height(){
        return m_heightEdit->text().toDouble();
    };

private:
    QLineEdit* m_valueEdit;
    QLineEdit* m_heightEdit;
    QDialogButtonBox* m_buttonBox;
};

#endif //EDGE_THIVKNESS_SOLVE_DLG_H
