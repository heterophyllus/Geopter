#ifndef FILEIO_H
#define FILEIO_H


#include <string>
#include <vector>

namespace geopter {


class OpticalSystem;


class FileIO
{
public:
    FileIO();
    ~FileIO();

    /** Save OpticalSystem as JSON file. */
    static bool save_to_json(const OpticalSystem& opt_sys, std::string json_path);

    /** Load OpticalSystem data from JSON file */
    static bool load_from_json(OpticalSystem& opt_sys, std::string json_path);

};

} //namespace geopter

#endif // FILEIO_H
