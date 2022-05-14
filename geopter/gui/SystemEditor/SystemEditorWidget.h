#ifndef SYSTEM_EDITOR_WIDGET_H
#define SYSTEM_EDITOR_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include "LensDataEditor/LensDataTableView.h"
#include "SpecEditor/SpecEditorDlg.h"

/** The widget inside of central dock that includes "Specification" button and lens data spreadsheet view */
class SystemEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemEditorWidget(std::shared_ptr<geopter::OpticalSystem> optsys, QWidget *parent = nullptr);
    ~SystemEditorWidget();

    void setOpticalSystem(std::shared_ptr<geopter::OpticalSystem> optsys);

    LensDataTableView* lensDataView();

public slots:
    void showSpecEditorDlg();

private:
    QGridLayout *m_gridLayout;
    QPushButton *m_specButton;
    LensDataTableView* m_lensDataView;
    std::shared_ptr<geopter::OpticalSystem> m_opticalSystem;
};

#endif // SYSTEM_EDITOR_WIDGET_H
