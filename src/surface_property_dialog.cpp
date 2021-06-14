#include "surface_property_dialog.h"
#include "ui_surface_property_dialog.h"

SurfacePropertyDialog::SurfacePropertyDialog(std::shared_ptr<OpticalModel> opt_model, int surface_index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfacePropertyDialog),
    opt_model_(opt_model),
    surface_index_(surface_index)
{
    ui->setupUi(this);

    this->setWindowTitle("Surface " + QString::number(surface_index_));

    QObject::connect(ui->listSurfaceProperty, SIGNAL(currentRowChanged(int)), this, SLOT(showStackedPropertyTab(int)));
    ui->listSurfaceProperty->setCurrentRow(0);
    ui->stackSurfaceProperty->setCurrentIndex(0);

    QObject::connect(ui->comboSurfaceType, SIGNAL(currentIndexChanged(int)), this, SLOT(initializeCoefsTable(int)));

    QObject::connect(ui->comboApertureType, SIGNAL(currentIndexChanged(int)), this, SLOT(showStackedApertureTab(int)));
    ui->comboApertureType->setCurrentIndex(0);
    ui->stackApertureType->setCurrentIndex(0);

    ui->editCircularRadius->setValidator(new QDoubleValidator(0, 10000, 1.0, this));
    ui->editCircularRadius->setText(QString::number(1.0));

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

    syncUiWithModel();
}

SurfacePropertyDialog::~SurfacePropertyDialog()
{
    opt_model_.reset();
    delete ui;
}

void SurfacePropertyDialog::showStackedPropertyTab(int i)
{
    ui->stackSurfaceProperty->setCurrentIndex(i);
    ui->stackSurfaceProperty->show();
}

void SurfacePropertyDialog::showStackedApertureTab(int i)
{
    ui->stackApertureType->setCurrentIndex(i);
    ui->stackApertureType->show();
}


void SurfacePropertyDialog::onAccept()
{
    syncModelWithUi();
    accept();
}

void SurfacePropertyDialog::syncModelWithUi()
{
    /* Surface Profile */
    double r = ui->tableProfileData->item(0,0)->text().toDouble();
    double cv = 1.0/r;

    int surface_type = ui->comboSurfaceType->currentIndex();

    if( surface_type == 0 ){ // Sphere
        opt_model_->seq_model()->surface(surface_index_)->set_profile(std::make_unique<Spherical>(cv));
    }
    else if(surface_type == 1){ // Even Asphere
        double k = ui->tableProfileData->item(1,0)->text().toDouble();

        std::vector<double> coefs(10, 0.0);
        for(int i = 0; i < 10; i++){
            coefs[i] = ui->tableProfileData->item(i+2, 0)->text().toDouble();
        }

        opt_model_->seq_model()->surface(surface_index_)->set_profile(std::make_unique<EvenPolynomial>(cv,k,coefs));
    }

    /* aperture */
    int aperture_type = ui->comboApertureType->currentIndex();

    if(aperture_type == 0){ //None
        opt_model_->seq_model()->surface(surface_index_)->remove_clear_aperture();
    }
    else if(aperture_type == 1){ // Circular
        double ap_radius = ui->editCircularRadius->text().toDouble();
        opt_model_->seq_model()->surface(surface_index_)->set_clear_aperture(std::make_unique<Circular>(ap_radius));
    }

    /* Decenter */
    // to be implemented
}


void SurfacePropertyDialog::syncUiWithModel()
{
    /* Surface Profile */
    std::string surface_type_name = opt_model_->seq_model()->surface(surface_index_)->profile()->name();

    if(surface_type_name == "SPH"){

        ui->comboSurfaceType->setCurrentIndex(0);

        initializeCoefsTable(0);

        double r = opt_model_->seq_model()->surface(surface_index_)->profile()->radius();
        setValueToCell(ui->tableProfileData, 0, 0, r);

    }else if(surface_type_name == "ASP"){

        ui->comboSurfaceType->setCurrentIndex(1);

        initializeCoefsTable(1);

        double r = opt_model_->seq_model()->surface(surface_index_)->profile()->radius();
        double k = dynamic_cast<EvenPolynomial*>(opt_model_->seq_model()->surface(surface_index_)->profile())->conic();
        setValueToCell(ui->tableProfileData, 0, 0, r);
        setValueToCell(ui->tableProfileData, 1, 0, k);

        for(int i = 0; i < 10; i++){
            double coef = dynamic_cast<EvenPolynomial*>(opt_model_->seq_model()->surface(surface_index_)->profile())->coef(i);
            setValueToCell(ui->tableProfileData, i+2, 0, coef);
        }
    }


    /* Aperture */
    if(opt_model_->seq_model()->surface(surface_index_)->clear_aperture()){
        ui->comboApertureType->setCurrentIndex(1);
        double ap_radius = opt_model_->seq_model()->surface(surface_index_)->clear_aperture()->max_dimension();
        ui->editCircularRadius->setText(QString::number(ap_radius));

    }else{ // None
        ui->comboApertureType->setCurrentIndex(0);
    }

}


void SurfacePropertyDialog::initializeCoefsTable(int type)
{
    ui->tableProfileData->setColumnCount(1);
    QStringList hLabels;
    hLabels << "Value";
    ui->tableProfileData->setHorizontalHeaderLabels(hLabels);

    if(type == 0){ // Sphere
        ui->tableProfileData->setRowCount(1);

        QStringList labels;
        labels << "R";
        ui->tableProfileData->setVerticalHeaderLabels(labels);

    }else if(type == 1){
        ui->tableProfileData->setRowCount(12); // R + k + coef10

        QStringList labels;
        labels << "R" << "k";
        for(int i = 1; i <= 10; i++){
            labels << ("A" + QString::number((i+1)*2) );
        }
        ui->tableProfileData->setVerticalHeaderLabels(labels);
    }

    /* set items */
    for(int i = 0; i < ui->tableProfileData->rowCount(); i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        ui->tableProfileData->setItem(i,0,item);
    }

}

void SurfacePropertyDialog::setValueToCell(QTableWidget* table, int row, int col, double val)
{
    QTableWidgetItem *item = table->item(row, col);
    if(!item){
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }

    item->setTextAlignment(Qt::AlignRight);

    item->setText(QString::number(val, 'g', 8));

}
