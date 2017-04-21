#ifndef _HADRON_PDFS_SOLVER_BASE
#define _HADRON_PDFS_SOLVER_BASE

#include "include/SpinBasis.h"
#include "include/Action.h"

// A solver is a function
//
// lattice source field (fermion, propagator, ...) --> lattice propagator
//
// If everything is implemented correctly, the resulting propagator satisfies
//
// result = D[lattice gauge field]^-1(lattice source field)
//
// where D is the dirac operator.

class Solver{
public:
    Solver( const Action &S, const GroupXML_t solverParams, const multi1d<LatticeColorMatrix> &U, SPIN_BASIS desired_basis=SPIN_BASIS::DIRAC_PAULI);
    LatticePropagator operator()(const LatticePropagator& source);
    // TODO: It would be nice to require the ability to spit out XML.
private:
    Handle< SystemSolver<LatticeFermion> > solver;
    SPIN_BASIS default_spin_basis;
    SPIN_BASIS desired_spin_basis;
    
};

#endif /* end of include guard: _HADRON_PDFS_SOLVER_BASE */
