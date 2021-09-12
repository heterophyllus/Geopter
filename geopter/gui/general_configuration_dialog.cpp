#include "general_configuration_dialog.h"
#include "ui_general_configuration_dialog.h"

#include <QFileDialog>
#include "optical.h"


GeneralConfigurationDialog::GeneralConfigurationDialog(std::shared_ptr<geopter::OpticalSystem> opt_sys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralConfigurationDialog),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);

    updateCatalogList();

    QObject::connect(ui->buttonBrowseAGF, SIGNAL(clicked()), this, SLOT(browseAgfFiles()));
}

GeneralConfigurationDialog::~GeneralConfigurationDialog()
{
    delete ui;
}

void GeneralConfigurationDialog::browseAgfFiles()
{
    // open file selection dialog
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("select AGF"),
                                                          QApplication::applicationDirPath(),
                                                          tr("AGF files(*.agf);;All Files(*.*)"));
    if(filePaths.empty()){
        return;
    }

    std::vector< std::string > agf_paths;
    for(auto &qspath : filePaths) {
        agf_paths.push_back(qspath.toStdString());
    }

    opt_sys_->material_lib()->load_agf_files(agf_paths);

    updateCatalogList();

}


void GeneralConfigurationDialog::updateCatalogList()
{
    QStringList catalogNames;

    for(int i = 0; i < opt_sys_->material_lib()->catalog_count(); i++){
        catalogNames << QString().fromStdString(opt_sys_->material_lib()->glass_catalog(i)->name());
    }

    ui->listCatalogs->clear();
    ui->listCatalogs->addItems(catalogNames);
}
