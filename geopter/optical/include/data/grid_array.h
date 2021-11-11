#ifndef GRID_ARRAY_H
#define GRID_ARRAY_H

#include <vector>
#include <memory>


namespace geopter{

template<class T>
using GridArray = std::vector< std::vector<T> > ;


} //namespace geopter


#endif //GRID_ARRAY_H
