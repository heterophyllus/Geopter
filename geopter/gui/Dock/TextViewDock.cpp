#include "TextViewDock.h"

#include <QApplication>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include <iostream>
#include <sstream>

TextViewDock::TextViewDock(QString label, OpticalSystem* sys, QWidget *parent) :
    ads::CDockWidget(label, parent),
    m_opticalSystem(sys)
{
    this->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    this->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    this->setMinimumSize(300,200);

    // Text Edit
    m_textEdit = new QTextEdit;
    m_textEdit->setLineWrapMode(QTextEdit::NoWrap);
    this->setWidget(m_textEdit);
    //m_textEdit->setLineWrapColumnOrWidth(200);

    // Tool bar
    m_toolbar = new QToolBar(this);

    auto actionUpdate = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_BrowserReload ),"Update");
    auto actionSetting = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_FileDialogContentsView ),"Setting");
    auto actionSave = m_toolbar->addAction(QApplication::style()->standardIcon( QStyle::SP_DialogSaveButton ),"Save");

    //m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setToolBar(m_toolbar);

    QObject::connect(actionUpdate,  SIGNAL(triggered()), this, SLOT(updateContent()));
    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
}

TextViewDock::~TextViewDock()
{
    delete m_toolbar;
    delete m_textEdit;
    m_settingDlgPtr.reset();
}

void TextViewDock::setStringStreamToText(std::ostringstream& ss)
{
    m_textEdit->setPlainText(QString::fromStdString(ss.str()));
}

void TextViewDock::clear()
{
    m_textEdit->clear();
}


void TextViewDock::showSettingDlg()
{
    if(m_settingDlgPtr){
        if(m_settingDlgPtr->exec() == QDialog::Accepted){
            this->updateContent();
        }
    }
}

void TextViewDock::saveToFile()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save to txt"),"",tr("Text file(*.txt);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    QFile textFile(filePath);
    textFile.open(QIODevice::WriteOnly);

    QTextStream out(&textFile);
    out << m_textEdit->toPlainText() << Qt::endl;

    textFile.close();
}

void TextViewDock::updateContent()
{
    m_settingDlgPtr->updateParentDockContent();
}
