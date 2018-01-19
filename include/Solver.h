#ifndef _HADRON_PDFS_SOLVER_BASE
#define _HADRON_PDFS_SOLVER_BASE

// A solver is a function
//
// lattice source field (fermion, propagator, ...) --> lattice propagator
//
// If everything is implemented correctly, the resulting propagator satisfies
//
// result = D[lattice gauge field]^-1(lattice source field)
//
// where D is the dirac operator.

#include "include/Action.h"

class Solver{
public:
    Solver( const Action &S, GroupXML_t solver_parameters, const multi1d<LatticeColorMatrix> &U );
    void operator()(LatticePropagator& result, const LatticePropagator& source);
    // TODO: It would be nice to require the ability to spit out XML.
private:
    Handle< SystemSolver<LatticeFermion> > D_inv;
};

#endif /* end of include guard: _HADRON_PDFS_SOLVER_BASE */
