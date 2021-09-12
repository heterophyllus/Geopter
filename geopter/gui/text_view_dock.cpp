#include "text_view_dock.h"

#include <QDialog>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include <iostream>
#include <sstream>

TextViewDock::TextViewDock(QString label, QWidget *parent) :
    ads::CDockWidget(label, parent)
{
    this->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    this->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    //this->resize(300,200);
    this->setMinimumSize(300,200);

    // Text Edit
    textEdit_ = new QTextEdit;
    this->setWidget(textEdit_);
    //textEdit_->setLineWrapColumnOrWidth(200);
    textEdit_->setLineWrapMode(QTextEdit::NoWrap);

    // Set monospaced font to format text
    //QFont fnt("Courier",16);
    //textEdit_->setCurrentFont(fnt);

    // Tool bar
    toolBar_ = new QToolBar(this);
    auto actionSetting = toolBar_->addAction("Setting");
    auto actionSave = toolBar_->addAction("Save");
    this->setToolBar(toolBar_);

    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
}

TextViewDock::~TextViewDock()
{
    delete textEdit_;
    delete toolBar_;

    settingDlgPtr_.reset();
}

void TextViewDock::possessDlg(std::unique_ptr<QDialog> dlg)
{
    settingDlgPtr_ = std::move(dlg);
}

void TextViewDock::setStringStreamToText(std::ostringstream& ss)
{
    textEdit_->setPlainText(QString::fromStdString(ss.str()));
}

void TextViewDock::clear()
{
    textEdit_->clear();
}


void TextViewDock::showSettingDlg()
{
    if(settingDlgPtr_){
        settingDlgPtr_->exec();
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
    out << textEdit_->toPlainText() << Qt::endl;

    textFile.close();
}
