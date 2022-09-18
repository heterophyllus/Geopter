#include "FieldCurvatureDlg.h"
#include "ui_fieldcurvaturedlg.h"


#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>

FieldCurvatureDlg::FieldCurvatureDlg(OpticalSystem* sys, AnalysisViewDock *parent) :
    AnalysisSettingDlg(sys, parent),
    ui(new Ui::FieldCurvatureDlg),
    m_parentDock(parent)
{
    ui->setupUi(this);

    m_renderer = new RendererQCP(m_parentDock->customPlot());

    ui->scaleEdit->setValidator(new QDoubleValidator(0.0, 100.0, 4, this));
    ui->scaleEdit->setText(QString::number(0.3));
    ui->numRaysEdit->setValidator(new QIntValidator(2, 100, this));
    ui->numRaysEdit->setText(QString::number(20));

    // FIXME : It is unclear why the buttonBox of this dialog itself doesn't work, though that of other dialogs work fine.
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

FieldCurvatureDlg::~FieldCurvatureDlg()
{
    delete m_renderer;
    delete ui;
}

void FieldCurvatureDlg::updateParentDockContent()
{
    m_opticalSystem->update_model();
    double maxField = m_opticalSystem->optical_spec()->field_of_view()->max_field();

    double scale = ui->scaleEdit->text().toDouble();
    int numRays = ui->numRaysEdit->text().toInt();

    Astigmatism *ast = new Astigmatism(m_opticalSystem);
    auto plotData = ast->plot(numRays);

    std::ostringstream oss;
    plotData->print(oss);

    m_renderer->clear();
    m_renderer->draw_plot(plotData);
    m_renderer->set_x_axis_range(-scale, scale);
    m_renderer->set_y_axis_range(0.0, maxField);
    m_renderer->set_x_axis_label(plotData->x_axis_label());
    m_renderer->set_y_axis_label(plotData->y_axis_label());
    m_renderer->draw_x_axis();
    m_renderer->draw_y_axis();
    m_renderer->update();

    delete ast;

    m_parentDock->setText(oss);
}
