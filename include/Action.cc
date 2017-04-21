#include "standard_includes.h"
#include "include/Action.h"

Action::Action(const GroupXML_t &ferm, SPIN_BASIS default_spin_basis){
    // S=Handle<const WilsonTypeFermAct<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > >(TheWilsonTypeFermActFactory::Instance().createObject(action_id,fermxml,path));
    spin_basis=default_spin_basis;
};