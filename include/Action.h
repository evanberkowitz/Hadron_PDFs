#ifndef _HADRON_PDFS_ACTION
#define _HADRON_PDFS_ACTION

#include "include/SpinBasis.h"

class Solver;

class Action {
friend class Solver;
public:
    Action(const GroupXML_t &ferm, SPIN_BASIS default_spin_basis=SPIN_BASIS::CHROMA);
private:
    Handle<const WilsonTypeFermAct<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > > S;
    SPIN_BASIS spin_basis;
};


#endif /* end of include guard: _HADRON_PDFS_ACTION */
