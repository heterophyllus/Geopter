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

    void set_data(const std::vector<double>& x, const std::vector<double>& y);
    void set_data(int i, double x, double y){x_data_[i] = x;y_data_[i] = y;}
    void add_data(double x, double y);
    void set_name(const std::string& name) { name_ = name;}
    void set_render_color(const Rgb& color) { render_color_ = color;}
    void set_line_width(double lw) { line_width_ = lw;}
    void set_line_style(int ls){ line_style_ = ls;}
    void set_x_label(const std::string& label){x_label_ = label;}
    void set_y_label(const std::string& label){y_label_ = label;}
    void reserve(int n){x_data_.reserve(n); y_data_.reserve(n);}
    void resize(int n){x_data_.resize(n); y_data_.resize(n);}

    int data_count() const { return x_data_.size();}
    void get_data(double* x, double* y) const;
    std::string name() const{return name_;}
    Rgb render_color() const{return render_color_;}
    double line_width() const{return line_width_;}
    int line_style() const { return line_style_;}
    std::string x_label() const { return x_label_;}
    std::string y_label() const { return y_label_;}

    void get_x_range(double* lower, double* higher) const;
    void get_y_range(double* lower, double* higher) const;

    void get_data(std::vector<double>& xdata, std::vector<double>& ydata);

    const std::vector<double>& x_data() const { return x_data_;}
    const std::vector<double>& y_data() const { return y_data_;}

    void print(std::ostringstream& oss);

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
