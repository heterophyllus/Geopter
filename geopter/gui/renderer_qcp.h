#ifndef RENDERERQCP_H
#define RENDERERQCP_H

#include "renderer/renderer.h"
#include "data/plot_data.h"
#include "qcustomplot.h"

using namespace geopter;


class RendererQCP : public Renderer
{
public:
    RendererQCP(QCustomPlot* customPlot);
    ~RendererQCP();

    void Clear() override;
    void Update() override;

    void SetGridLayout(int rows, int cols) override;
    void SetCurrentCell(int row, int col) override;

    void DrawPlot(const std::shared_ptr<PlotData> plotdata) override;

    void DrawHist2d(const Eigen::MatrixXd& Z, int type, int colormap);

    void DrawLine(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void DrawPolyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void DrawPolyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void DrawDots(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double dot_size) override;

    QCPRange CurrentXaxisRange();
    QCPRange CurrentYaxisRange();

    void SetXaxisRange(double xmin, double xmax) override;
    void SetYaxisRange(double ymin, double ymax) override;

    void SetXaxisLabel(std::string label) override;
    void SetYaxisLabel(std::string label) override;

    void SetAspectRatio(double h_per_v) override;

    void SetMouseInteraction(bool state);

private:
    QCustomPlot* customPlot_;

    inline QColor rgb_to_QColor(const Rgb& rgb);
};


QColor RendererQCP::rgb_to_QColor(const Rgb& rgb)
{
    return QColor(rgb.r*255.0, rgb.g*255.0, rgb.b*255.0);
}

#endif // RENDERERQCP_H
