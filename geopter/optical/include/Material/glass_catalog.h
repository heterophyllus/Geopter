#ifndef GLASSCATALOG_H
#define GLASSCATALOG_H

#include <memory>
#include <vector>
#include <string>

namespace geopter {

class Glass;

class GlassCatalog
{
public:
    GlassCatalog();
    ~GlassCatalog();

    /**
     * @brief get glass property data from AGF file
     * @param agf_path AGF file path
     * @return success
     */
    bool load_agf(std::string agf_path);

    /** Get glass object pointer.  If not found, return nullptr */
    std::shared_ptr<Glass> glass(std::string product_name);

    /** Return glass ptr at the index */
    std::shared_ptr<Glass> glass(int i);

    /** Get supplier's name of the catalog */
    std::string name() const;

    /** Return number of glasses */
    int glass_count() const;

    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    /** supplier name */
    std::string name_;
    std::vector< std::shared_ptr<Glass> > glasses_;
};

} //namespace geopter


#endif // GLASSCATALOG_H
