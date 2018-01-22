#ifndef _HADRON_PDFS_ACTION
#define _HADRON_PDFS_ACTION

#include "include/SpinBasis.h"

class Solver;

typedef Handle<FermState<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > > State;
typedef Handle< SystemSolver<LatticeFermion> > System_Solver;

class Action {
friend class Solver;
public:
    Action(XMLReader& file, const std::string& path);
    State state(const multi1d<LatticeColorMatrix> &U ) const;
    System_Solver solver(State &state, GroupXML_t solver_parameters) const;
private:
    Handle<const WilsonTypeFermAct<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > > S;
    System_Solver sys_solver;
};


#endif /* end of include guard: _HADRON_PDFS_ACTION */
