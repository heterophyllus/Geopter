#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include "LensDataTableView.h"
#include "SystemEditor/SystemDataConstant.h"
#include "SystemEditor/Delegate/NotEditableDelegate.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include "SystemEditor/Delegate/OneByteDelegate.h"
#include "SurfacePropertyDlg.h"
#include "SystemEditor/SolveDlg/SolveSelectionDlg.h"
#include "SystemEditor/SolveDlg/EdgeThicknessSolveDlg.h"
#include "SystemEditor/SolveDlg/OverallLengthSolveDlg.h"

LensDataTableView::LensDataTableView(std::shared_ptr<OpticalSystem> opt_sys, QWidget *parent) :
    QTableView(parent),
    m_opt_sys(opt_sys)
{
    m_model = new LensDataTableModel(m_opt_sys, this);
    this->setModel(m_model);

    // delegates
    this->setItemDelegateForColumn(LensDataColumn::Label,          new OneByteDelegate);
    this->setItemDelegateForColumn(LensDataColumn::SurfaceType,    new NotEditableDelegate(this));
    this->setItemDelegateForColumn(LensDataColumn::Radius,         new FloatDelegate(4, true, this));
    this->setItemDelegateForColumn(LensDataColumn::RadiusSolve,    new NotEditableDelegate(this));
    this->setItemDelegateForColumn(LensDataColumn::Thickness,      new FloatDelegate(4, true, this));
    this->setItemDelegateForColumn(LensDataColumn::ThicknessSolve, new NotEditableDelegate(this));
    this->setItemDelegateForColumn(LensDataColumn::SemiDiameter,   new FloatDelegate(4, true, this));
    this->setItemDelegateForColumn(LensDataColumn::Aperture,       new NotEditableDelegate(this));

    // header labels
    this->setGridHeaderview(Qt::Horizontal, 2);
    GridTableHeaderView *hHeader = this->gridHeaderview(Qt::Horizontal);

    hHeader->setSpan(0, LensDataColumn::Radius, 1, 2);
    hHeader->setSpan(0, LensDataColumn::Thickness, 1, 2);
    hHeader->setCellLabel(0, LensDataColumn::Label,        "Label");
    hHeader->setCellLabel(0, LensDataColumn::SurfaceType,  "Surface Type");
    hHeader->setCellLabel(0, LensDataColumn::Radius,       "Radius");
    hHeader->setCellLabel(0, LensDataColumn::Thickness,    "Thickness");
    hHeader->setCellLabel(0, LensDataColumn::Material,     "Material");
    hHeader->setCellLabel(0, LensDataColumn::Mode,         "Mode");
    hHeader->setCellLabel(0, LensDataColumn::SemiDiameter, "Semi Diameter");
    hHeader->setCellLabel(0, LensDataColumn::Aperture,     "Aperture");

    this->setColumnWidth(LensDataColumn::RadiusSolve, 1);
    this->setColumnWidth(LensDataColumn::ThicknessSolve, 1);

    // mouse click
    this->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->verticalHeader(),  SIGNAL(customContextMenuRequested(const QPoint &)), this,  SLOT(showContextMenuOnHeader()) );

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuOnCell()));

    QObject::connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClick(const QModelIndex &)));
}

LensDataTableView::~LensDataTableView()
{
    delete m_model;
}

LensDataTableModel* LensDataTableView::lensDataModel()
{
    return m_model;
}

void LensDataTableView::setGridHeaderview(Qt::Orientation orientation, int levels)
{
    GridTableHeaderView *header;

    if (orientation == Qt::Horizontal) {
        header = new GridTableHeaderView(orientation, levels, model()->columnCount());
        setHorizontalHeader(header);
        #if QT_VERSION >= 0x050000
                QObject::connect(header, &GridTableHeaderView::sectionPressed, this,
                    &LensDataTableView::horizontalHeaderSectionPressed);
        #else
                QObject::connect(header, SIGNAL(sectionPressed(int, int)), this,
                    SLOT(horizontalHeaderSectionPressed(int, int)));
        #endif
    }
    else {
        header = new GridTableHeaderView(orientation, model()->rowCount(), levels);
        setVerticalHeader(header);
        #if QT_VERSION >= 0x050000
            QObject::connect(header, &GridTableHeaderView::sectionPressed, this,
                    &LensDataTableView::verticalHeaderSectionPressed);
        #else
            QObject::connect(header, SIGNAL(sectionPressed(int, int)), this,
                    SLOT(verticalHeaderSectionPressed(int, int)));
        #endif
  }
}

GridTableHeaderView* LensDataTableView::gridHeaderview(Qt::Orientation orientation)
{
    GridTableHeaderView *header;
    if (orientation == Qt::Horizontal) {
        header = qobject_cast<GridTableHeaderView *>(horizontalHeader());
    } else {
        header = qobject_cast<GridTableHeaderView *>(verticalHeader());
    }
    return header;
}

void LensDataTableView::horizontalHeaderSectionPressed(int beginSection, int endSection)
{
    clearSelection();
    QAbstractItemView::SelectionMode oldSelectionMode = selectionMode();
    setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i = beginSection; i < endSection + 1; ++i){
        selectColumn(i);
    }
    setSelectionMode(oldSelectionMode);
}

void LensDataTableView::verticalHeaderSectionPressed(int beginSection, int endSection)
{
    clearSelection();
    QAbstractItemView::SelectionMode oldSelectionMode = selectionMode();
    setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i = beginSection; i < endSection + 1; ++i){
        selectRow(i);
    }
    setSelectionMode(oldSelectionMode);
}

void LensDataTableView::showContextMenuOnHeader()
{
    const int row = this->selectedIndexes().at(0).row();

    if(row == 0){ // object surface

    }else if(row == this->model()->rowCount()-1){ // image surface
        QMenu contextMenu;
        QAction *action1 = contextMenu.addAction("Insert");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertRow()));
        contextMenu.exec(QCursor::pos());
    }else{
        QMenu contextMenu;

        QAction *action1 = contextMenu.addAction("Insert");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertRow()));

        QAction *action2 = contextMenu.addAction("Remove");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(removeRow()));

        QAction *action3 = contextMenu.addAction("Set stop");
        QObject::connect(action3, SIGNAL(triggered()), this, SLOT(setStop()));

        QAction *action4 = contextMenu.addAction("Property");
        QObject::connect(action4, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

        contextMenu.exec(QCursor::pos());
    }
}

void LensDataTableView::showContextMenuOnCell()
{
    QMenu contextMenu;
    QAction *action1 = contextMenu.addAction("Property");
    QObject::connect(action1, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));
    QAction *action2 = contextMenu.addAction("Solve");
    QObject::connect(action2, SIGNAL(triggered()), this, SLOT(showSolveSelectionDlg()));

    contextMenu.exec(QCursor::pos());
}

void LensDataTableView::insertRow()
{
    const int row = this->selectedIndexes().at(0).row();
    this->lensDataModel()->insertRows(row, 1);
}

void LensDataTableView::removeRow()
{
    const int row = this->selectedIndexes().at(0).row();
    this->lensDataModel()->removeRows(row, 1);
}

void LensDataTableView::setStop()
{
    const int row = this->selectedIndexes().at(0).row();
    this->lensDataModel()->setStop(row);
}

void LensDataTableView::showSurfacePropertyDlg()
{
    const int si = this->selectedIndexes().at(0).row();

    if( 0 > si || si > (this->model()->rowCount()-1) ){
        qDebug() << "LensDataTableView::showSurfacePropertyDlg()  out of surface index si= " << si;
        return;
    }

    SurfacePropertyDlg dlg(m_opt_sys, si, this);
    dlg.syncUiWithSystem();
    if(dlg.exec() == QDialog::Accepted){
        m_opt_sys->update_model();
    }
}

void LensDataTableView::showSolveSelectionDlg(int si)
{
    bool isLastSurface = false;
    if(si == m_opt_sys->optical_assembly()->gap_count()-1){
        isLastSurface = true;
    }

    int currentSolveIndex = m_opt_sys->optical_assembly()->gap(si)->solve_type();
    if(currentSolveIndex < 0){
        currentSolveIndex = 0;
    }
    SolveSelectionDlg solveDlg(currentSolveIndex, isLastSurface, this);

    if(solveDlg.exec() == QDialog::Accepted){
        int selectedIndex = solveDlg.selectedIndex();

        if(selectedIndex == Solve::EdgeThickness){
            EdgeThicknessSolveDlg *edgeDlg = new EdgeThicknessSolveDlg();
            if(edgeDlg->exec() == QDialog::Accepted){
                auto solve = std::make_unique<EdgeThicknessSolve>(si, edgeDlg->value(), edgeDlg->height());
                if(solve->check(m_opt_sys.get())){
                    m_opt_sys->optical_assembly()->gap(si)->set_solve(std::move(solve));
                    m_opt_sys->update_model();
                }else{
                    solve.reset();
                }
            }
            delete edgeDlg;
        }else if(selectedIndex == Solve::OverallLength){
            OverallLengthSolveDlg *dlg = new OverallLengthSolveDlg();
            if(dlg->exec() == QDialog::Accepted){
                auto solve = std::make_unique<OverallLengthSolve>(si, dlg->value(), dlg->startSurface(), dlg->endSurface());
                if(solve->check(m_opt_sys.get())){
                    m_opt_sys->optical_assembly()->gap(si)->set_solve(std::move(solve));
                    m_opt_sys->update_model();
                }
            }
            delete dlg;
        }else if(selectedIndex == Solve::ParaxialImageDistance){
            // no dialog
            auto solve = std::make_unique<ParaxialImageSolve>(si);
            if(solve->check(m_opt_sys.get())){
                m_opt_sys->optical_assembly()->gap(si)->set_solve(std::move(solve));
                m_opt_sys->update_model();
            }
        }
    }


}

void LensDataTableView::onDoubleClick(const QModelIndex &index)
{
    switch (index.column()) {
    case LensDataColumn::SurfaceType:
    case LensDataColumn::Mode:
    case LensDataColumn::SemiDiameter:
    case LensDataColumn::Aperture:
        showSurfacePropertyDlg();
        break;
    //case LensDataColumn::RadiusSolve:
    case LensDataColumn::ThicknessSolve:
        showSolveSelectionDlg(index.row());
        break;
    }
}
