#include "renderer_qcp.h"

#include <iostream>


RendererQCP::RendererQCP(QCustomPlot* customPlot)
{
    customPlot_ = customPlot;
    current_cell_index_ = 0;
    current_line_width_ = 2;
}

RendererQCP::~RendererQCP()
{
    customPlot_ = nullptr;
}

void RendererQCP::Clear()
{
    customPlot_->clearGraphs();
    customPlot_->clearItems();
    customPlot_->clearPlottables();
}

void RendererQCP::Update()
{
    customPlot_->update();
    customPlot_->replot();
}

void RendererQCP::SetGridLayout(int rows, int cols)
{
    customPlot_->plotLayout()->clear();

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QCPAxisRect *axisRect = new QCPAxisRect(customPlot_);
            axisRect->setObjectName("axis rect (" + QString::number(i) + "," + QString::number(j) + ")");
            customPlot_->plotLayout()->addElement(i, j, axisRect);
            axisRect->setLayer("axes");
        }
    }

}

void RendererQCP::SetCurrentCell(int row, int col)
{
    current_cell_index_ = customPlot_->plotLayout()->rowColToIndex(row, col);
}


void RendererQCP::DrawPlot(std::shared_ptr<PlotData> plotdata)
{
    const int num_graphs = plotdata->NumberOfGraphs();

    for (int i = 0; i < num_graphs; i++) {
        int ls = plotdata->GetGraph(i)->LineStyle();
        double lw = plotdata->GetGraph(i)->LineWidth();
        Rgb color = plotdata->GetGraph(i)->RenderColor();

        std::vector<double> x, y;
        plotdata->GetGraph(i)->GetData(x, y);

        DrawPolyline(x, y, color, ls, lw);
    }
}

void RendererQCP::DrawHist2d(const Eigen::MatrixXd &Z, int type, int cmap)
{
    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    QCPColorMap *colorMap = new QCPColorMap(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    int nx = Z.rows();
    int ny = Z.cols();

    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, nx), QCPRange(0, ny));
    colorMap->setInterpolate(false);


    for (int yIndex = 0; yIndex < ny; ++yIndex)
    {
      for (int xIndex = 0; xIndex < nx; ++xIndex)
      {
          double z = Z(yIndex, xIndex); //(x,y) (col, row)

          if(std::isnan(z)){
              z = 0.0;
          }

          if(type == 0){ // linear
              //z = z;
          }
          else if(type == 1){ //logarithmic
              z = std::log10(z);
          }

          colorMap->data()->setCell(xIndex, yIndex, z);
      }
    }

    if(cmap == 0){
        colorMap->setGradient(QCPColorGradient::gpGrayscale);
    }else{
        colorMap->setGradient(QCPColorGradient::gpJet);
    }

    colorMap->rescaleDataRange(true);

    customPlot_->replot();
}


void RendererQCP::DrawLine(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style, double line_width)
{
    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);

    //QCPCurve* line = new QCPCurve(_customPlot->xAxis, _customPlot->yAxis);
    QCPCurve* line = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    line->setPen( QPen(rgb_to_QColor(color), line_width) );

    switch (line_style) {
    case Renderer::LineStyle::Solid:
        line->setLineStyle(QCPCurve::lsLine);
        line->setScatterStyle(QCPScatterStyle::ssNone);
        break;

    case Renderer::LineStyle::Dots:
        line->setLineStyle(QCPCurve::lsNone);
        line->setScatterStyle(QCPScatterStyle::ssDisc);
        break;
    }


    QVector<double> x(2);
    QVector<double> y(2);

    x[0] = p1(0);
    x[1] = p2(0);
    y[0] = p1(1);
    y[1] = p2(1);

    line->setData(x,y);
    line->setVisible(true);
}

void RendererQCP::DrawPolyline(const std::vector<Eigen::Vector2d> &pts, const Rgb& color, int line_style, double line_width)
{
    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    //QCPCurve* polyline = new QCPCurve(_customPlot->xAxis, _customPlot->yAxis);
    QCPCurve* polyline = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    polyline->setPen(QPen(rgb_to_QColor(color), line_width));

    switch (line_style) {
    case Renderer::LineStyle::Solid:
        polyline->setLineStyle(QCPCurve::lsLine);
        polyline->setScatterStyle(QCPScatterStyle::ssNone);
        break;

    case Renderer::LineStyle::Dots:
        polyline->setLineStyle(QCPCurve::lsNone);
        polyline->setScatterStyle(QCPScatterStyle::ssDisc);
        break;
    }

    int pointCount = pts.size();
    QVector<QCPCurveData> curveData(pointCount);

    for(int i = 0; i < pointCount; i++){
        curveData[i] = QCPCurveData(i, pts[i](0), pts[i](1));
    }

    polyline->data()->set(curveData, true);
    polyline->setVisible(true);

}

void RendererQCP::DrawPolyline(const std::vector<double> &x, const std::vector<double> &y, const Rgb& color, int line_style, double line_width)
{
    int pointCount = x.size();
    QVector<QCPCurveData> curveData(pointCount);

    for(int i = 0; i < pointCount; i++)
    {
        curveData[i] = QCPCurveData(i, x[i], y[i]);
    }

    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    QCPCurve* polyline = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    polyline->setPen(QPen(rgb_to_QColor(color), line_width));

    switch (line_style) {
    case Renderer::LineStyle::Solid:
        polyline->setLineStyle(QCPCurve::lsLine);
        polyline->setScatterStyle(QCPScatterStyle::ssNone);
        break;

    case Renderer::LineStyle::Dots:
        polyline->setLineStyle(QCPCurve::lsNone);
        polyline->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, line_width));
        break;
    }

    polyline->data()->clear();
    polyline->data()->set(curveData, true);
    polyline->setVisible(true);
}

void RendererQCP::DrawDots(const std::vector<double> &x, const std::vector<double> &y, const Rgb &color, double dot_size)
{
    int pointCount = x.size();
    QVector<QCPCurveData> dotsData(pointCount);

    for(int i = 0; i < pointCount; i++)
    {
        dotsData[i] = QCPCurveData(i, x[i], y[i]);
    }

    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    QCPCurve* dots = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    dots->setPen(QPen(rgb_to_QColor(color), dot_size));
    dots->setLineStyle(QCPCurve::lsNone);
    dots->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, dot_size));

    dots->data()->clear();
    dots->data()->set(dotsData, true);
    dots->setVisible(true);
}

QCPRange RendererQCP::CurrentXaxisRange()
{
    return customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom)->range();
}

QCPRange RendererQCP::CurrentYaxisRange()
{
    return customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->range();
}

void RendererQCP::SetXaxisRange(double xmin, double xmax)
{
    //_customPlot->xAxis->setRange(xmin, xmax);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom)->setRange(xmin, xmax);
}

void RendererQCP::SetYaxisRange(double ymin, double ymax)
{
    //_customPlot->yAxis->setRange(ymin, ymax);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->setRange(ymin, ymax);
}


void RendererQCP::SetXaxisLabel(std::string label)
{
    QString xLabel = QString().fromStdString(label);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom)->setLabel(xLabel);
}

void RendererQCP::SetYaxisLabel(std::string label)
{
    QString yLabel = QString().fromStdString(label);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->setLabel(yLabel);
}

void RendererQCP::SetAspectRatio(double h_per_v)
{
    customPlot_->yAxis->setScaleRatio(customPlot_->xAxis, h_per_v);
    QCPAxis *xAxis = customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom);
    QCPAxis *yAxis = customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft);

    xAxis->setScaleRatio(yAxis, h_per_v);
}

void RendererQCP::SetMouseInteraction(bool state)
{
    customPlot_->setInteraction(QCP::iRangeDrag,   state);
    customPlot_->setInteraction(QCP::iRangeZoom,   state);
    customPlot_->setInteraction(QCP::iSelectItems, state);
}

