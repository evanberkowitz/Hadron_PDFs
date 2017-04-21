#include "standard_includes.h"

#include "include/Solver.h"
#include "include/SpinBasis.h"

Solver::Solver( const Action &action, const GroupXML_t solver_parameters, const multi1d<LatticeColorMatrix> &U, SPIN_BASIS desired_basis) {
    Handle< SystemSolver<LatticeFermion> > result;
    Handle<FermState<LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> > > state = (action.S)->createState(U);
    
    try{
        solver=Handle< SystemSolver<LatticeFermion> >((action.S)->qprop(state,solver_parameters));
    }
    catch(const std::string& e){
        QDPIO::cerr << "Caught Exception creating Solver instance XML: " << e << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    
    default_spin_basis=action.spin_basis;
    desired_spin_basis=desired_basis;
};

LatticePropagator Solver::operator()(const LatticePropagator& source){
    LatticePropagator result=zero;
    LatticeFermion chi,psi;
    for(unsigned int c=0; c < Nc; c++){
        for(unsigned int s=0; s < Ns; s++){
            chi=zero;
            psi=zero;
            Chroma::PropToFerm(source, chi, c, s);
            (*solver)(psi, chi);
            Chroma::FermToProp(psi, result, c, s);
        }
    }
    
    spin_basis_transform(result, desired_spin_basis, default_spin_basis);
    return result;
}