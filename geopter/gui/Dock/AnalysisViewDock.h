#ifndef ANALYSIS_RESULT_DOCK_H
#define ANALYSIS_RESULT_DOCK_H

#include "DockWidget.h"

#include <memory>

#include <QToolBar>
#include "qcustomplot.h"

#include "AnalysisDlg/AnalysisSettingDlg.h"
#include "optical.h"
using namespace geopter;

class AnalysisViewDock : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit AnalysisViewDock(QString label, OpticalSystem* sys, bool textonly= false, QWidget *parent = nullptr);
    ~AnalysisViewDock();

    void setCurrentTab(int i){
        m_tabWidget->setCurrentIndex(i);
    }

    void setText(std::ostringstream& oss){
        m_textEdit->setPlainText(QString::fromStdString(oss.str()));
    }

    QCustomPlot* customPlot(){
        return m_customPlot;
    }

    QTextEdit* textEdit(){
        return m_textEdit;
    }

    template<class D> void createSettingDialog() {
        m_settingDlgPtr = std::make_unique<D>(m_opticalSystem, this);
    }

public slots:
    void showSettingDlg();

    void saveToFile();

    void copyToClipboard();

    virtual void updateContent();

protected:
    QTabWidget*  m_tabWidget;
    QCustomPlot* m_customPlot;
    QTextEdit*   m_textEdit;
    QToolBar*    m_toolbar;
    std::unique_ptr<AnalysisSettingDlg> m_settingDlgPtr;
    OpticalSystem *m_opticalSystem;
    bool m_textOnly;
};


#endif //ANALYSIS_RESULT_DOCK_H
