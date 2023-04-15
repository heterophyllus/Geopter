#ifndef GEOPTER_POINT_2D_H
#define GEOPTER_POINT_2D_H

#include <string>
#include <vector>
#include <sstream>
#include "renderer/rgb.h"


namespace geopter{

class Graph2d
{
public:
    Graph2d();
    Graph2d(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color= rgb_black, int line_style=0, double line_width=1.0);
    virtual ~Graph2d();

    void SetData(const std::vector<double>& x, const std::vector<double>& y);
    void SetData(int i, double x, double y){x_data_[i] = x;y_data_[i] = y;}
    void AddData(double x, double y);
    void SetName(const std::string& name) { name_ = name;}
    void SetRenderColor(const Rgb& color) { render_color_ = color;}
    void SetLineWidth(double lw) { line_width_ = lw;}
    void SetLineStyle(int ls){ line_style_ = ls;}
    void SetXLabel(const std::string& label){x_label_ = label;}
    void SetYLabel(const std::string& label){y_label_ = label;}
    void Reserve(int n){x_data_.reserve(n); y_data_.reserve(n);}
    void Resize(int n){x_data_.resize(n); y_data_.resize(n);}

    int NumberOfData() const { return x_data_.size();}
    void GetData(double* x, double* y) const;
    std::string Name() const{return name_;}
    Rgb RenderColor() const{return render_color_;}
    double LineWidth() const{return line_width_;}
    int LineStyle() const { return line_style_;}
    std::string XLabel() const { return x_label_;}
    std::string YLabel() const { return y_label_;}

    void GetXRange(double* lower, double* higher) const;
    void GetYRange(double* lower, double* higher) const;

    void GetData(std::vector<double>& xdata, std::vector<double>& ydata);

    const std::vector<double>& XData() const { return x_data_;}
    const std::vector<double>& YData() const { return y_data_;}

    void Print(std::ostringstream& oss);

private:
    std::vector<double> x_data_;
    std::vector<double> y_data_;
    std::string name_;
    Rgb render_color_;
    int plot_style_;
    int line_style_;
    double line_width_;
    std::string x_label_;
    std::string y_label_;
};


} //namespace geopter

#endif 
