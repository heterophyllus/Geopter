#include "SystemEditorWidget.h"
#include "SpecEditorDlg.h"
#include <QClipboard>
#include <QShortcut>


using namespace geopter;

SystemEditorWidget::SystemEditorWidget(std::shared_ptr<geopter::OpticalSystem> optsys, QWidget *parent) :
    QWidget(parent),
    m_opticalSystem(optsys)
{
    m_gridLayout = new QGridLayout(this);

    // spec
    m_specButton = new QPushButton(this);
    m_specButton->setText("Specifications");

    m_lensSpreadSheet = new LensSpreadSheet(m_opticalSystem, this);
    m_gridLayout->addWidget(m_specButton,0,0,1,1);
    m_gridLayout->addWidget(m_lensSpreadSheet,1,0,1,2);

    QObject::connect(m_specButton, SIGNAL(clicked()), this, SLOT(showSpecEditorDlg()) );

}

SystemEditorWidget::~SystemEditorWidget()
{
    delete m_specButton;
    delete m_lensSpreadSheet;
    delete m_gridLayout;
}

void SystemEditorWidget::setOpticalSystem(std::shared_ptr<geopter::OpticalSystem> optsys)
{
    m_opticalSystem = optsys;
}

LensSpreadSheet* SystemEditorWidget::lensSpreadSheet()
{
    return m_lensSpreadSheet;
}

void SystemEditorWidget::showSpecEditorDlg()
{

    SpecEditorDlg dlg(nullptr);
    dlg.loadData(m_opticalSystem);


    if(dlg.exec() == QDialog::Accepted){
        dlg.applyData(m_opticalSystem);
        m_opticalSystem->update_model();
        m_lensSpreadSheet->reload();
    }
}
