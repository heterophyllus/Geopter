#ifndef TEXT_VIEW_DOCK_H
#define TEXT_VIEW_DOCK_H

#include <memory>

#include <QDialog>
#include <QTextEdit>

#include "DockWidget.h"

/** This dock is used to show an analysis result in text format. */
class TextViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit TextViewDock(QString label, QWidget *parent = nullptr);
    ~TextViewDock();

    /** Clear text */
    void clear();

public slots:
    virtual void updateText();
    virtual void setStringStreamToText(std::ostringstream& ss);

    void showSettingDlg();

    /** Save text to .txt file */
    void saveToFile();

protected:
    QTextEdit *m_textEdit;
    QToolBar* m_toolbar;

    std::unique_ptr<QDialog> m_settingDlgPtr;
};

#endif // TEXT_VIEW_DOCK_H
