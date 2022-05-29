#ifndef OVERALL_LENGTH_SOLVE_DLG_H
#define OVERALL_LENGTH_SOLVE_DLG_H

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QValidator>
#include <QDialogButtonBox>

class OverallLengthSolveDlg : public QDialog
{
    Q_OBJECT

public:
    OverallLengthSolveDlg(int start=1, int end=2, double value=10.0, QWidget* parent = nullptr) : QDialog(parent){
        //this->setAttribute(Qt::WA_DeleteOnClose, true);
        this->setWindowTitle("Overall Length Solve");

        QLabel *valueLabel = new QLabel("Value");
        QLabel *startLabel = new QLabel("Start");
        QLabel *endLabel   = new QLabel("End");
        m_startSurfaceEdit = new QLineEdit;
        m_startSurfaceEdit->setValidator(new QIntValidator(0, 100, this));
        m_startSurfaceEdit->setText(QString::number(start));
        m_endSurfaceEdit = new QLineEdit;
        m_endSurfaceEdit->setValidator(new QIntValidator(0, 100, this));
        m_endSurfaceEdit->setText(QString::number(end));
        m_valueEdit = new QLineEdit;
        m_valueEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));
        m_valueEdit->setText(QString::number(value));

        m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(startLabel,         0, 0);
        layout->addWidget(endLabel,           1, 0);
        layout->addWidget(valueLabel,         2, 0);
        layout->addWidget(m_startSurfaceEdit, 0, 1);
        layout->addWidget(m_endSurfaceEdit,   1, 1);
        layout->addWidget(m_valueEdit,        2, 1);
        layout->addWidget(m_buttonBox,        3, 0, 1, 2);

        this->setLayout(layout);
    }

    int startSurface(){
        return m_startSurfaceEdit->text().toInt();
    }
    int endSurface(){
        return m_endSurfaceEdit->text().toInt();
    }
    double value(){
        return m_valueEdit->text().toDouble();
    }

private:
    QLineEdit* m_startSurfaceEdit;
    QLineEdit* m_endSurfaceEdit;
    QLineEdit* m_valueEdit;
    QDialogButtonBox* m_buttonBox;

};

#endif //OVERALL_LENGTH_SOLVE_DLG_H
