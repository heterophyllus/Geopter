#ifndef SYSTEM_EDITOR_WIDGET_H
#define SYSTEM_EDITOR_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include "LensSpreadSheet.h"
#include "SpecEditorDlg.h"

/**  This widget is contained inside SystemEditorDock
 *
 *   It includes "Specification" button and lens spreadsheet
*/
class SystemEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemEditorWidget(std::shared_ptr<geopter::OpticalSystem> optsys, QWidget *parent = nullptr);
    ~SystemEditorWidget();

    void setOpticalSystem(std::shared_ptr<geopter::OpticalSystem> optsys);

    LensSpreadSheet* lensSpreadSheet();

public slots:
    void showSpecEditorDlg();

private:
    QGridLayout *m_gridLayout;
    QPushButton *m_specButton;
    LensSpreadSheet* m_lensSpreadSheet;
    std::shared_ptr<geopter::OpticalSystem> m_opticalSystem;
};

#endif // SYSTEM_EDITOR_WIDGET_H
