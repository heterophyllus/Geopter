#include "Element/element_model.h"

#include "Element/lens.h"
//#include "Element/mirror.h"
#include "Element/stop.h"
#include "Element/dummy_interface.h"
#include "Element/air_gap.h"
#include "System/optical_system.h"
#include "Assembly/optical_assembly.h"
#include "Material/material.h"


using namespace geopter;

ElementModel::ElementModel(OpticalSystem* sys) :
    opt_sys_(sys)
{

}

ElementModel::~ElementModel()
{
    opt_sys_ = nullptr;
}

Element* ElementModel::element(int i) const
{   
    assert( i >= 0);

    if( i < (int)elements_.size()){
        return elements_[i].get();
    }else{
        return nullptr;
    }

}


int ElementModel::element_count() const
{
    return elements_.size();
}

void ElementModel::create()
{
    int num_gaps = opt_sys_->optical_assembly()->gap_count();
    int stop_index = opt_sys_->optical_assembly()->stop_index();

    clear();

    for(int gi = 0; gi < num_gaps; gi++){
        Gap* gap = opt_sys_->optical_assembly()->gap(gi);

        if(gap->material()->name() == "AIR") {
            if(gi == 0){
                // add dummy as Object
                auto dummy = std::make_unique<DummyInterface>(opt_sys_->optical_assembly()->surface(gi));
                elements_.push_back(std::move(dummy));

            }else{
                Gap* gap_before = opt_sys_->optical_assembly()->gap(gi-1);
                if(gap_before->material()->name() == "AIR"){
                    if(gi == stop_index){
                        // add stop
                        auto stop_elem = std::make_unique<Stop>(opt_sys_->optical_assembly()->surface(gi));
                        elements_.push_back(std::move(stop_elem));
                    }else{
                        // add dummy
                        auto dummy = std::make_unique<DummyInterface>(opt_sys_->optical_assembly()->surface(gi));
                        elements_.push_back(std::move(dummy));
                    }
                }
            }
        }
        else{
            // add lens
            Surface* s1 = opt_sys_->optical_assembly()->surface(gi);
            Surface* s2 = opt_sys_->optical_assembly()->surface(gi+1);
            auto lens = std::make_unique<Lens>(s1, s2, gap);
            elements_.push_back(std::move(lens));
        }
    }

}

void ElementModel::clear()
{
    if(!elements_.empty()){
        for(auto &e:elements_){
            e.reset();
        }
        elements_.clear();
    }
}


void ElementModel::update_model()
{
    create();
}
