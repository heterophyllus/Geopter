#ifndef GRID_ARRAY_H
#define GRID_ARRAY_H

#include <vector>
#include <limits>
#include "Eigen/Core"

namespace geopter{

template<class T>
class GridArray
{
public:
    GridArray();
    GridArray(int rows, int cols);
    ~GridArray();

    int rows() const { return rows_; }
    int cols() const { return cols_;}

    T at(int i, int j);
    void set_cell(int i, int j, T data);
    void resize(int rows, int cols);
    void fill(T val);

    /** Converto to matrix.  Only "double" type is valid, otherwise returns nan array */
    Eigen::MatrixXd to_matrix();

private:
    inline int row_col_to_index(int i, int j) const { return cols_*i + j; }

    int rows_;
    int cols_;
    std::vector<T> data_;
};

template<class T>
GridArray<T>::GridArray()
{
    rows_ = 0;
    cols_ = 0;
    data_.resize(0);
}

template<class T>
GridArray<T>::GridArray(int rows, int cols)
{
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows*cols);
}

template<class T>
GridArray<T>::~GridArray()
{
    data_.clear();
}

template<class T>
void GridArray<T>::resize(int rows, int cols)
{
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows*cols);
}

template<class T>
void GridArray<T>::fill(T val)
{
    int len = rows_*cols_;
    for(int i = 0; i < len; i++) {
        data_[i] = val;
    }
}

template<class T>
T GridArray<T>::at(int i, int j)
{
    return data_[row_col_to_index(i, j)];
}

template<class T>
void GridArray<T>::set_cell(int i, int j, T data)
{
    data_[row_col_to_index(i, j)] = data;
}


template<class T>
Eigen::MatrixXd GridArray<T>::to_matrix()
{
    Eigen::MatrixXd m(rows_, cols_);

    for(int i = 0; i < rows_; i++) {
        for(int j = 0; j < cols_; j++) {
            m(i,j) = data_[row_col_to_index(i, j)];
        }
    }

    return m;
}


} //namespace geopter


#endif //GRID_ARRAY_H
