#ifndef GEOPTER_TABLE_DATA_H
#define GEOPTER_TABLE_DATA_H

#include <variant>
#include <vector>
#include <string>

namespace geopter {

class TableData
{
public:
    TableData();
    TableData(int nrow, int ncol);

    std::variant<int, double, std::string>& data(int index);
    std::variant<int, double, std::string>& data(int row, int col);

    void init(int nrow, int ncol);
    int to_index(int row, int col);
    void generate_csv(const std::string& filepath);

private:
    int nrow_;
    int ncol_;
    std::vector< std::variant<int, double, std::string> > data_;

};


}


#endif //GEOPTER_TABLE_DATA_H

