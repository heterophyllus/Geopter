#ifndef GEOPTER_UTILITY_H
#define GEOPTER_UTILITY_H


#include <string>
#include <vector>

namespace geopter {

/** Utilities for general purpose */
class Utility
{
public:
    /** Split file path into components */
    static void split_path(const std::string& full_path, std::string& folder_path, std::string& file_base, std::string& ext);

    /** Split string with the deliminater */
    static std::vector<std::string> split(const std::string& s, char delim);

    /** Return true if the string starts with the sepecified prefix */
    static bool starts_with(const std::string& s, const std::string& prefix);

    /** Check whether specific string is contained */
    static bool contains(const std::string& s, const std::string& item);
};

}

#endif //GEOPTER_UTILITY_H
