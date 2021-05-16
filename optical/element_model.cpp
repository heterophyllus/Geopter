#include "element_model.h"
#include "optical_model.h"
#include "sequential_model.h"
#include "element.h"
#include "surface.h"
#include "gap.h"
#include "medium.h"
#include "glass.h"



using namespace geopter;

ElementModel::ElementModel(OpticalModel* opt_model) :
    opt_model_(opt_model)
{

}

ElementModel::~ElementModel()
{
    if(!elements_.empty()){
        for(auto &e : elements_){
            e.reset();
        }
        elements_.clear();
    }

}

Element* ElementModel::element(int i)
{
    int num_elems = (int)elements_.size();
    if(i >= 0 && i < num_elems){
        return elements_[i].get();
    }else if(i < 0){
        return elements_[num_elems + i].get();
    }else{
        return nullptr;
    }
}

int ElementModel::element_count()
{
    return (int)elements_.size();
}

void ElementModel::update_model()
{
    auto seq_model = opt_model_->seq_model();
    auto tfrms = seq_model->compute_global_coords(1);

    for(auto &e:elements_){
        e->update_size();
        e->sync_to_update(*seq_model);
        auto intrfc = e->reference_interface();
        int i=0;
        try {
            //i = seq_model->get_surface_index(intrfc);
        }  catch (...) {

        }
        if(i > 0){
            //e.tfrm = tfrms[i]
        }
    }

    sequence_elements();
}

void ElementModel::list_elements()
{
    for(int i = 0; i < (int)elements_.size(); i++){
        std::printf("%d : %s\n", i, elements_[i]->label().c_str());
    }
}

void ElementModel::sequence_elements()
{
    /*
    seq_model = self.opt_model.seq_model
            self.elements.sort(key=lambda e:
                               seq_model.ifcs.index(e.reference_interface()))
            # Make sure z_dir matches the sequential model. Used to get
            # the correct substrate offset.
            if hasattr(seq_model, 'z_dir'):
                for e in self.elements:
                    if hasattr(e, 'z_dir'):
                        e.z_dir = seq_model.z_dir[e.reference_idx()]
                        */


}

void ElementModel::elements_from_sequence()
{
    auto seq_model = opt_model_->seq_model();

    /*
    // if there are elements in the list already, just return
    if(elements_.size() > 0){
        return;
    }
    */
    if(!elements_.empty()){
        for(auto &e:elements_){
            e.reset();
        }
        elements_.clear();
    }

    int num_elements = 0;

    std::vector<Tfrm> tfrms = seq_model->compute_global_coords(1);
    int gap_count = seq_model->gap_count();
    Tfrm tfrm;
    for(int i = 0; i < gap_count; i++)
    {
        auto g = seq_model->gap(i);
        auto s1 = seq_model->surface(i);
        tfrm = tfrms[i];

        if(g->medium()->name() == "AIR"){
            if(i > 0){
                process_airgap(i,g,s1,tfrm,num_elements,true);
            }
        }
        else{ // a non-air medium
            // handle buried mirror, e.g. prism or Mangin mirror
            if(s1->interact_mode() == "reflect"){
                auto gp = seq_model->gap(i-1);
                if(gp->medium()->name() == g->medium()->name()){
                    continue;
                }
            }

            auto s2 = seq_model->surface(i+1);
            double sd = std::max(s1->semi_diameter(), s2->semi_diameter());
            std::shared_ptr<Lens> e = std::make_shared<Lens>(s1, s2, g, tfrm, i, i+1, sd);
            num_elements += 1;
            add_element(e);
        }
    }

    add_dummy_interface_at_image(tfrm);

}

void ElementModel::process_airgap(int i, Gap* g, Surface* s, Tfrm tfrm, int num_ele, bool add_ele)
{
    auto seq_model = opt_model_->seq_model();

    if(s->interact_mode() == "reflect" && add_ele)
    {
        double sd = s->semi_diameter();
        double z_dir = seq_model->z_dir(i);
        //mirror
    }
    else if(s->interact_mode() == "transmit"){
        bool add_dummy = false;
        std::string dummy_label = "";
        if(i == 0){
            add_dummy = true;
            dummy_label = "Object";
        }else{
            auto gp = seq_model->gap(i-1);
            if(gp->medium()->name() == "AIR"){
                add_dummy = true;
                if(seq_model->stop_surface() == i){
                    dummy_label = "Stop";
                }else{
                    //dummy_label = DummyInterface.label_format.format(i)
                    dummy_label = "";
                }
            }
        }

        if(add_dummy)
        {
            //tfrm = tfrm;
            double sd = s->semi_diameter();
            auto di = std::make_shared<DummyInterface>(s,i,sd,tfrm);
            di->set_label(dummy_label);
            add_element(di);
        }
    }
    /*
     * elif isinstance(s, thinlens.ThinLens) and add_ele:
            te = ThinElement(s, tfrm=tfrm, idx=i)
            num_ele += 1
            te.label = ThinElement.label_format.format(num_ele)
            self.add_element(te)
      */

    // add on AirGap
    auto ag = std::make_shared<AirGap>(g,s,i,tfrm);
    add_element(ag);
}

void ElementModel::add_element(std::shared_ptr<Element> e)
{
    elements_.push_back(e);

    /*
     * for ifc in e.interface_list():
            self.ifcs_dict[ifc] = e
        for g in e.gap_list():
            self.gap_dict[g] = e

    */
}


void ElementModel::add_dummy_interface_at_image(Tfrm tfrm)
{
    if(elements_.size() > 0 && element(-1)->label() == "Image"){
        return;
    }
    auto seq_model = opt_model_->seq_model();
    auto s = seq_model->surface(-1);
    int idx = seq_model->surface_count();
    double sd = s->semi_diameter();

    auto di = std::make_shared<DummyInterface>(s, idx, sd, tfrm);
    //DummyInterface* di = new DummyInterface(s.get(),idx,sd,tfrm);
    di->set_label("Image");
    add_element(di);
}


void ElementModel::relabel_airgaps()
{
    for(int i = 0; i < (int)elements_.size(); i++){
        auto e = elements_[i];
        if(e->label() == "AirGap"){
            std::string eb = elements_[i-1]->label();
            std::string ea = elements_[i+1]->label();
            e->set_label(eb+"-"+ea);
        }
    }
}
