#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <vector>
#include <string>
#include <memory>

#include "Material/material.h"
#include "Material/glass_catalog.h"

namespace geopter{


class MaterialLibrary
{
public:
    MaterialLibrary();
    ~MaterialLibrary();

    void clear();

    /** get glass catalog object pointer */
    GlassCatalog* glass_catalog(const std::string catname) const;
    GlassCatalog* glass_catalog(int i) const;

    /** Return number of loaded glass catalogs */
    int catalog_count() const;

    /** load AGF files  */
    bool load_agf_files(const std::vector<std::string>& agf_paths);

    std::shared_ptr<Material> find(std::string material_name);

private:
    std::vector< std::unique_ptr<GlassCatalog> > catalogs_;

};


} // namespace geopter

#endif // MATERIALLIBRARY_H
