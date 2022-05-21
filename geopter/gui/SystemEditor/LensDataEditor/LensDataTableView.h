#ifndef LENS_DATA_VIEW_H
#define LENS_DATA_VIEW_H

#include <QTableView>
#include "LensDataModel.h"
#include "GridTableHeaderView.h"
#include "OpticalSystemWrapper/QOpticalSystem.h"

/** Table view class for lens data */
class LensDataTableView : public QTableView
{
    Q_OBJECT

public:
    explicit LensDataTableView(std::shared_ptr<OpticalSystem> opt_sys, QWidget *parent = nullptr);
    ~LensDataTableView();

    void setGridHeaderview(Qt::Orientation orientation, int levels);
    GridTableHeaderView *gridHeaderview(Qt::Orientation orientation);

    LensDataModel* lensDataModel();

private slots:
    void onDoubleClick(const QModelIndex &index);
    void showContextMenuOnHeader();
    void showContextMenuOnCell();
    void insertRow();
    void removeRow();
    void setStop();
    void showSurfacePropertyDlg();
    void showSolveSelectionDlg();

    void horizontalHeaderSectionPressed(int beginSection, int endSection);
    void verticalHeaderSectionPressed(int beginSection, int endSection);

private:
    LensDataModel* m_model;
    std::shared_ptr<OpticalSystem> m_opt_sys;
};

#endif // LENS_DATA_VIEW_H
