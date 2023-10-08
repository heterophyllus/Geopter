#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include "LensDataTableView.h"
#include "SystemEditor/SystemDataConstant.h"
#include "SystemEditor/Delegate/NotEditableDelegate.h"
#include "SystemEditor/Delegate/FloatDelegate.h"
#include "SystemEditor/Delegate/OneByteDelegate.h"
#include "SurfacePropertyDlg.h"
#include "SystemEditor/LensDataEditor/SolveSettingDlg.h"

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

    hHeader->setVisible(true);

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
        QAction *action2 = contextMenu.addAction("Insert Multi");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertRow()));
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertMultipleRows()));
        contextMenu.exec(QCursor::pos());
    }else{
        QMenu contextMenu;

        QAction *action1 = contextMenu.addAction("Insert");
        QObject::connect(action1, SIGNAL(triggered()), this, SLOT(insertRow()));

        QAction *action2 = contextMenu.addAction("Insert Multi");
        QObject::connect(action2, SIGNAL(triggered()), this, SLOT(insertMultipleRows()));

        QAction *action3 = contextMenu.addAction("Remove");
        QObject::connect(action3, SIGNAL(triggered()), this, SLOT(removeRow()));

        QAction *action4 = contextMenu.addAction("Set stop");
        QObject::connect(action4, SIGNAL(triggered()), this, SLOT(setStop()));

        QAction *action5 = contextMenu.addAction("Property");
        QObject::connect(action5, SIGNAL(triggered()), this, SLOT(showSurfacePropertyDlg()));

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

void LensDataTableView::insertMultipleRows()
{
    const int row = this->selectedIndexes().at(0).row();

    int n = QInputDialog::getInt(this, tr("Input"), tr("Rows"), 1, 0, 100, 1);

    this->lensDataModel()->insertRows(row, n);
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
        m_opt_sys->UpdateModel();
    }
}

void LensDataTableView::showSolveSelectionDlg(int si)
{
    Gap* gap = m_opt_sys->GetOpticalAssembly()->GetGap(si);
    auto currentSolve = m_opt_sys->GetOpticalAssembly()->GetGap(si)->GetSolve();
    int currentSolveIndex = currentSolve->GetSolveType();
    if(currentSolveIndex < 0){
        currentSolveIndex = 0;
    }

    double param1, param2, param3, param4;
    currentSolve->GetParameters(&param1, &param2, &param3, &param4);


    SolveSettingDlg solveDlg(this);

    solveDlg.SetParameters(currentSolveIndex, param1, param2, param3, param4);

    if(solveDlg.exec() == QDialog::Accepted){
        int solvetype;
        double param1, param2, param3, param4;
        solveDlg.GetParameters(&solvetype, &param1, &param2, &param3, &param4);
        Solve* solve;

        if(solvetype == Solve::Fixed){
            solve = gap->CreateSolve<FixedSolve>();
        }else if(solvetype == Solve::EdgeThickness){
            solve = gap->CreateSolve<EdgeThicknessSolve>();
        }else if(solvetype == Solve::OverallLength){
            solve = gap->CreateSolve<OverallLengthSolve>();
        }else if(solvetype == Solve::MarginalHeight){
            solve = gap->CreateSolve<MarginalHeightSolve>();
        }else{
            qDebug() << "Unknown solve setting";
        }

        solve->SetGapIndex(si);
        solve->SetParameters(param1, param2, param3, param4);

        if( ! solve->Check(m_opt_sys.get())){
            gap->RemoveSolve();
        }else{
            gap->GetSolve()->Apply(m_opt_sys.get());
        }
    }

    m_opt_sys->UpdateModel();


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
