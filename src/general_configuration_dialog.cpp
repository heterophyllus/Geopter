#include "general_configuration_dialog.h"
#include "ui_general_configuration_dialog.h"

#include <QFileDialog>
#include "optical_core.h"

GeneralConfigurationDialog::GeneralConfigurationDialog(std::shared_ptr<geopter::OpticalModel> opt_model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralConfigurationDialog),
    opt_model_(opt_model)
{
    ui->setupUi(this);

    agfDirPath_ = QString().fromStdString(opt_model_->database()->agf_dir());
    ui->editAGFpath->setText(agfDirPath_);
    updateCatalogList();

    QObject::connect(ui->buttonBrowseAGF, SIGNAL(clicked()), this, SLOT(browseAgfDirectory()));
}

GeneralConfigurationDialog::~GeneralConfigurationDialog()
{
    delete ui;
}

void GeneralConfigurationDialog::browseAgfDirectory()
{
    QString currentAgfPath = agfDirPath_;

    QFileDialog::Options options =
          QFileDialog::ShowDirsOnly | QFileDialog::HideNameFilterDetails |
          QFileDialog::DontUseCustomDirectoryIcons;

    QString dirName = QFileDialog::getExistingDirectory( this, tr("Select AGF directory"), currentAgfPath, options);

    if(dirName.isEmpty()){
        return;
    }else{
        agfDirPath_ = dirName;
        ui->editAGFpath->setText(agfDirPath_);

        opt_model_->database()->load_all_agf(agfDirPath_.toStdString());
        updateCatalogList();
    }

}

void GeneralConfigurationDialog::setAgfDirectoryPath(QString currentDirPath)
{
    agfDirPath_ = currentDirPath;
}

QString GeneralConfigurationDialog::getAgfDirectoryPath()
{
    return agfDirPath_;
}

void GeneralConfigurationDialog::updateCatalogList()
{
    QStringList catalogNames;

    for(int i = 0; i < opt_model_->database()->catalog_count(); i++){
        catalogNames << QString().fromStdString(opt_model_->database()->glass_catalog(i)->name());
    }

    ui->listCatalogs->clear();
    ui->listCatalogs->addItems(catalogNames);
}
