#include "SurfacePropertyDlg.h"
#include "ui_SurfacePropertyDlg.h"

SurfacePropertyDlg::SurfacePropertyDlg(std::shared_ptr<OpticalSystem> opt_sys, int surface_index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfacePropertyDlg),
    opt_sys_(opt_sys),
    surface_index_(surface_index)
{
    ui->setupUi(this);

    this->setWindowTitle("Surface " + QString::number(surface_index_));

    QObject::connect(ui->surfaceProfileTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(showSurfaceProfileStackPage(int)));
    ui->sphericalRadiusEdit->setValidator(new QDoubleValidator(-10000.0, 10000.0, 8, this));

    QObject::connect(ui->apertureTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(showApertureStackPage(int)));
    ui->circularRadiusEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 8, this));

    ui->apertureStack->setCurrentIndex(0);

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

    initializeEvenAsphereDataTable();
    initializeOddAsphereDataTable();

    syncUiWithSystem();
}

SurfacePropertyDlg::~SurfacePropertyDlg()
{
    opt_sys_.reset();
    delete ui;
}

void SurfacePropertyDlg::showSurfaceProfileStackPage(int surfaceProfileType)
{
    ui->surfaceProfileStack->setCurrentIndex(surfaceProfileType);
    ui->surfaceProfileStack->show();
}

void SurfacePropertyDlg::showApertureStackPage(int apertureType)
{
    ui->apertureStack->setCurrentIndex(apertureType);
    ui->apertureStack->show();
}

void SurfacePropertyDlg::onAccept()
{
    syncSystemWithUi();
    accept();
}

void SurfacePropertyDlg::syncSystemWithUi()
{
    //-----> surface profile
    int surface_type = ui->surfaceProfileTypeCombo->currentIndex();

    if( surface_type == 0 ){ // Sphere
        double r = ui->sphericalRadiusEdit->text().toDouble();
        double cv = 1.0/r;
        //opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->set_profile(std::make_unique<Spherical>(cv));
        opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->SetProfile<Spherical>(cv);
    }
    else if(surface_type == 1){ // Even Asphere
        double r = ui->evenAsphereDataTable->item(0,0)->text().toDouble();
        double cv = 1.0/r;
        double k = ui->evenAsphereDataTable->item(1,0)->text().toDouble();

        std::vector<double> coefs(10, 0.0);
        for(int i = 0; i < 10; i++){
            coefs[i] = ui->evenAsphereDataTable->item(i+2, 0)->text().toDouble();
        }

        opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->SetProfile<EvenPolynomial>(cv,k,coefs);
    }
    else if(surface_type == 2){
        double r = ui->oddAsphereDataTable->item(0,0)->text().toDouble();
        double cv = 1.0/r;
        double k = ui->oddAsphereDataTable->item(1,0)->text().toDouble();

        std::vector<double> coefs(10, 0.0);
        for(int i = 0; i < 10; i++){
            coefs[i] = ui->oddAsphereDataTable->item(i+2, 0)->text().toDouble();
        }

        opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->SetProfile<OddPolynomial>(cv,k,coefs);
    }

    //-----> aperture
    int aperture_type = ui->apertureTypeCombo->currentIndex();

    if(aperture_type == 0){ //None
        opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->RemoveClearAperture();
    }
    else if(aperture_type == 1){ // Circular
        double ap_radius = ui->circularRadiusEdit->text().toDouble();
        opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->SetClearAperture<Circular>(ap_radius, ap_radius);
    }

    // decenter
    // not implemented
}


void SurfacePropertyDlg::syncUiWithSystem()
{
    //==================================
    // surface profile
    std::string surface_type_name = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->ProfileName();

    if(surface_type_name == "SPH"){
        ui->surfaceProfileTypeCombo->setCurrentIndex(0);
        ui->surfaceProfileStack->setCurrentIndex(0);
        ui->surfaceProfileStack->show();

        double r = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Radius();
        ui->sphericalRadiusEdit->setText(QString::number(r));
        setValueToCell(ui->evenAsphereDataTable, 0, 0, r);

    }else if(surface_type_name == "ASP"){
        ui->surfaceProfileTypeCombo->setCurrentIndex(1);
        ui->surfaceProfileStack->setCurrentIndex(1);
        ui->surfaceProfileStack->show();
        initializeEvenAsphereDataTable();

        double r = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Radius();
        double k = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Profile<EvenPolynomial>()->Conic();
        setValueToCell(ui->evenAsphereDataTable, 0, 0, r);
        setValueToCell(ui->evenAsphereDataTable, 1, 0, k);

        for(int i = 0; i < 10; i++){
            double coef = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Profile<EvenPolynomial>()->GetNthTerm(i);
            setValueToCell(ui->evenAsphereDataTable, i+2, 0, coef);
        }
        ui->sphericalRadiusEdit->setText(QString::number(r));

    }else if(surface_type_name == "ODD"){
        ui->surfaceProfileTypeCombo->setCurrentIndex(2);
        ui->surfaceProfileStack->setCurrentIndex(2);
        ui->surfaceProfileStack->show();
        initializeOddAsphereDataTable();

        double r = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Profile<OddPolynomial>()->Radius();
        double k = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Profile<OddPolynomial>()->Conic();
        setValueToCell(ui->oddAsphereDataTable, 0, 0, r);
        setValueToCell(ui->oddAsphereDataTable, 1, 0, k);

        for(int i = 0; i < 10; i++){
            double coef = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->Profile<OddPolynomial>()->GetNthTerm(i);
            setValueToCell(ui->oddAsphereDataTable, i+2, 0, coef);
        }
        ui->sphericalRadiusEdit->setText(QString::number(r));
    }


    //=====================================================
    // aperture
    if(opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->IsAperture<Circular>()){ // Circular

        ui->apertureTypeCombo->setCurrentIndex(1);
        double ap_radius = opt_sys_->GetOpticalAssembly()->GetSurface(surface_index_)->GetClearAperture<Circular>()->MaxDimension();
        ui->circularRadiusEdit->setText(QString::number(ap_radius));

    }else{ // None
        ui->apertureTypeCombo->setCurrentIndex(0);
    }

}


void SurfacePropertyDlg::initializeEvenAsphereDataTable()
{
    QStringList hLabels = {"Value"};
    ui->evenAsphereDataTable->setColumnCount(hLabels.size());
    ui->evenAsphereDataTable->setHorizontalHeaderLabels(hLabels);

    QStringList vLabels;
    vLabels << "R" << "k";
    for(int i = 1; i <= 10; i++){
        vLabels << ("A" + QString::number((i+1)*2) );
    }
    ui->evenAsphereDataTable->setRowCount(vLabels.size()); // R + k + coef10
    ui->evenAsphereDataTable->setVerticalHeaderLabels(vLabels);


    // set items
    for(int i = 0; i < ui->evenAsphereDataTable->rowCount(); i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        ui->evenAsphereDataTable->setItem(i,0,item);
    }


}

void SurfacePropertyDlg::initializeOddAsphereDataTable()
{
    QStringList hLabels = {"Value"};
    ui->oddAsphereDataTable->setColumnCount(hLabels.size());
    ui->oddAsphereDataTable->setHorizontalHeaderLabels(hLabels);

    QStringList vLabels;
    vLabels << "R" << "k";
    for(int i = 1; i <= 10; i++){
        vLabels << ("A" + QString::number(i+2) ); // A3, A4, A5...
    }
    ui->oddAsphereDataTable->setRowCount(vLabels.size()); // R + k + coef10
    ui->oddAsphereDataTable->setVerticalHeaderLabels(vLabels);


    // set items
    for(int i = 0; i < ui->oddAsphereDataTable->rowCount(); i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        ui->oddAsphereDataTable->setItem(i,0,item);
    }
}


void SurfacePropertyDlg::setValueToCell(QTableWidget* table, int row, int col, double val)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }

    item->setTextAlignment(Qt::AlignRight);
    item->setText(QString::number(val, 'g', 8));
}
