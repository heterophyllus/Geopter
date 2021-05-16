#ifndef GENERAL_CONFIGURATION_DIALOG_H
#define GENERAL_CONFIGURATION_DIALOG_H

#include <memory>

#include <QDialog>
#include <QString>

namespace geopter {
class OpticalModel;
}

namespace Ui {
class GeneralConfigurationDialog;
}

class GeneralConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralConfigurationDialog(std::shared_ptr<geopter::OpticalModel> opt_model, QWidget *parent = nullptr);
    ~GeneralConfigurationDialog();

    void setAgfDirectoryPath(QString currentDirPath);
    QString getAgfDirectoryPath();

private slots:
    void browseAgfDirectory();

private:
    Ui::GeneralConfigurationDialog *ui;

    std::shared_ptr<geopter::OpticalModel> opt_model_;
    QString agfDirPath_;

    void updateCatalogList();
};

#endif // GENERAL_CONFIGURATION_DIALOG_H
