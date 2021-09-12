#ifndef GENERAL_CONFIGURATION_DIALOG_H
#define GENERAL_CONFIGURATION_DIALOG_H

#include <memory>

#include <QDialog>
#include <QString>

namespace geopter {
class OpticalSystem;
}

namespace Ui {
class GeneralConfigurationDialog;
}

class GeneralConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralConfigurationDialog(std::shared_ptr<geopter::OpticalSystem> opt_sys, QWidget *parent = nullptr);
    ~GeneralConfigurationDialog();

    QString getAgfDirectoryPath();

private slots:
    void browseAgfFiles();

private:
    void updateCatalogList();

    Ui::GeneralConfigurationDialog *ui;
    std::shared_ptr<geopter::OpticalSystem> opt_sys_;

};

#endif // GENERAL_CONFIGURATION_DIALOG_H
