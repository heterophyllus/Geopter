#ifndef TEXT_VIEW_DOCK_H
#define TEXT_VIEW_DOCK_H


#include "DockWidget.h"

#include <QTextEdit>
#include <memory>


/**
 * This dock is used to show an analysis result in text format.
 *
 * Each analysis dock has the following functions.
 *    - Setting: Modify analysis setting. When the button is clicked, the setting dialog will appear.
 *
 */
class TextViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit TextViewDock(QString label, QWidget *parent = nullptr);
    ~TextViewDock();

    /** Clear text */
    void clear();

    /** Show text in stringstream */
    void setStringStreamToText(std::ostringstream& ss);

    void possessDlg(std::unique_ptr<QDialog> dlg);

public slots:
    void showSettingDlg();

    /** Save text to .txt file */
    void saveToFile();

private:
    QTextEdit *textEdit_;
    QToolBar* toolBar_;

    std::unique_ptr<QDialog> settingDlgPtr_;
};

#endif // TEXT_VIEW_DOCK_H
