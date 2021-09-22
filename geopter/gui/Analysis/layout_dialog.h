#ifndef LAYOUT_DIALOG_H
#define LAYOUT_DIALOG_H

#include <QDialog>
#include "plot_view_dock.h"


namespace Ui {
class LayoutDialog;
}

class LayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LayoutDialog(PlotViewDock *parent = nullptr);
    ~LayoutDialog();

public:
    void getSettings(bool *doDrawRefRays, bool *doDrawFanRays, int *numberOfFanRays= nullptr);

//private slots:
    //void onAccept();

private:
    Ui::LayoutDialog *ui;
    PlotViewDock* m_parentDock;

};

#endif // LAYOUT_DIALOG_H
