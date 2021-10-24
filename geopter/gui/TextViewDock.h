#ifndef TEXT_VIEW_DOCK_H
#define TEXT_VIEW_DOCK_H

#include <memory>
#include <QTextEdit>

#include "DockWidget.h"
#include "AnalysisDlg/AnalysisSettingDlg.h"

#include "optical.h"
using namespace geopter;

/** This dock is used to show an analysis result in text format. */
class TextViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit TextViewDock(QString label, OpticalSystem* sys, QWidget *parent = nullptr);
    ~TextViewDock();

    /** Clear text */
    void clear();

    void setStringStreamToText(std::ostringstream& ss);

    template<class D> void createSettingDialog() {
        m_settingDlgPtr = std::make_unique<D>(m_opticalSystem, this);
    }

public slots:
    virtual void updateContent();

    void showSettingDlg();

    /** Save text to .txt file */
    void saveToFile();

protected:
    QTextEdit *m_textEdit;
    QToolBar* m_toolbar;
    OpticalSystem *m_opticalSystem;
    std::unique_ptr<AnalysisSettingDlg> m_settingDlgPtr;
};

#endif // TEXT_VIEW_DOCK_H
