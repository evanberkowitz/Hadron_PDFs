#include "standard_includes.h"
#include "include/Action.h"

Action::Action(XMLReader& fermxml, const std::string& path){
    QDPIO::cout << "Constructing Action" << std::endl;
    QDPIO::cout << "    Path = " << path << std::endl;
    
    std::string action_id_path=(path+"/FermAct");
    std::string action_id="";
    try{
        read(fermxml, action_id_path, action_id);
    }
    catch(const std::string& e){
      QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
      QDP_abort(1);
    }
    QDPIO::cout << "    FermAct = " << action_id << std::endl;

    try{
      //get fermion action:
      S=Handle<const WilsonTypeFermAct<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > >(TheWilsonTypeFermActFactory::Instance().createObject(action_id.c_str(),fermxml,path.c_str()));
    }
    catch(const std::string& e){
      QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
      QDP_abort(1);
    }

};

State Action::state(const multi1d<LatticeColorMatrix> &U ) const {
    QDPIO::cout << "Action::state..." << std::endl;
    State state = S->createState(U);
    return state;
}

System_Solver Action::solver(State &state, GroupXML_t solver_parameters) const {
    QDPIO::cout << "Action::solver..." << std::endl;
    return S->qprop(state, solver_parameters);
}