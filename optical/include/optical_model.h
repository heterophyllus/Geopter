#ifndef OPTICALMODEL_H
#define OPTICALMODEL_H

#include <string>
#include <memory>

namespace geopter {

class Database;
class SequentialModel;
class ParaxialModel;
class ElementModel;
class OpticalSpec;

class OpticalModel
{
public:
    OpticalModel(std::shared_ptr<Database> database=nullptr);
    ~OpticalModel();

    void set_name(std::string name);
    std::string name() const;

    Database* get_database() const;
    std::shared_ptr<Database> database() const;
    void set_database(std::shared_ptr<Database> d);

    OpticalSpec* optical_spec() const;
    ParaxialModel* paraxial_model() const;
    SequentialModel* seq_model() const;
    ElementModel* elem_model() const;

    /** Set simplest optical system.
     *  Surfaces: Obj, STO, Img
     *  Specs: single field, single wvl
     */
    void set_default_model();

    void update_model();

private:
    std::shared_ptr<Database> database_;

    std::unique_ptr<SequentialModel> seq_model_;
    std::unique_ptr<ParaxialModel> parax_model_;
    std::unique_ptr<ElementModel> elem_model_;
    std::unique_ptr<OpticalSpec> optical_spec_;

    std::string name_;
};

} //namespace geopter

#endif // OPTICALMODEL_H
