#ifndef SOLVE_SELECTION_DLG_H
#define SOLVE_SELECTION_DLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStringList>
#include <QDebug>

class SolveSelectionDlg : public QDialog
{
    Q_OBJECT
public:
    SolveSelectionDlg(int defaultIndex= 0, bool isLastSurface= false, QWidget* parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Solve");

        QStringList items({"Edge Thickness", "Overall Length"});
        if(isLastSurface) items << "Paraxial Image";

        m_combo = new QComboBox;
        m_combo->addItems(items);
        m_combo->setCurrentIndex(defaultIndex);

        m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(m_combo);
        layout->addWidget(m_buttonBox);

        setLayout(layout);
    }

    ~SolveSelectionDlg(){
        try {
            delete m_combo;
            delete m_buttonBox;
        }  catch (...) {
            qDebug() << "Delete error: SolveSelectionDlg";
        }
    }

    int selectedIndex() const {
        return m_combo->currentIndex();
    }

private:
    QComboBox* m_combo;
    QDialogButtonBox* m_buttonBox;
};


#endif //SOLVE_SELECTION_DLG_H
