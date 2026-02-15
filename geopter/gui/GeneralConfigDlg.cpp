#include "GeneralConfigDlg.h"
#include "ui_GeneralConfigDlg.h"

#include <QFileDialog>
#include "optical.h"


GeneralConfigDlg::GeneralConfigDlg(std::shared_ptr<geopter::OpticalSystem> opt_sys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralConfigDlg),
    opt_sys_(opt_sys)
{
    ui->setupUi(this);

    updateCatalogList();

    QObject::connect(ui->buttonBrowseAGF, SIGNAL(clicked()), this, SLOT(browseAgfFiles()));
}

GeneralConfigDlg::~GeneralConfigDlg()
{
    delete ui;
}

void GeneralConfigDlg::browseAgfFiles()
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

    opt_sys_->GetMaterialLib()->LoadAgfFiles(agf_paths);

    updateCatalogList();

}


void GeneralConfigDlg::updateCatalogList()
{
    QStringList catalogNames;

    for(int i = 0; i < opt_sys_->GetMaterialLib()->NumberOfCatalogs(); i++){
        catalogNames << QString().fromStdString(opt_sys_->GetMaterialLib()->GetGlassCatalog(i)->Name());
    }

    ui->listCatalogs->clear();
    ui->listCatalogs->addItems(catalogNames);
}
