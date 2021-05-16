#ifndef FILEIO_H
#define FILEIO_H


#include <string>
#include <vector>

namespace geopter {


class OpticalModel;

/**
 * @brief Manager class for external file io.
 *
 * @note - This class also provides some utility functions.
 *       - JSON file is operated using the following library.
 *          https://github.com/nlohmann/json
 */
class FileIO
{
public:
    FileIO();

    /** Save OpticalModel as JSON file. */
    static bool save_to_json(const OpticalModel& opt_model, std::string json_path);

    /** Load OpticalModel data from JSON file */
    static bool load_from_json(OpticalModel& opt_model, std::string json_path);

    /** Split file path into components */
    static void split_path(const std::string& full_path, std::string& folder_path, std::string& file_base, std::string& ext);

    /** Split string with the deliminater */
    static std::vector<std::string> split(const std::string& s, char delim);

    /** Return true if the string starts with the sepecified prefix */
    static bool starts_with(const std::string& s, const std::string& prefix);

    /** Check whether specific string is contained */
    static bool contains(const std::string& s, const std::string& item);

    /** Round to the float number in n digits */
    static double round_n(double number, int n);
    static double floor_n(double number, int n);
    static double ceil_n(double number, int n);

};

} //namespace geopter

#endif // FILEIO_H
