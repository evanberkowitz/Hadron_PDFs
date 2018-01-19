#include "standard_includes.h"

#include "include/Solver.h"

Solver::Solver( const Action &S, GroupXML_t solver_parameters, const multi1d<LatticeColorMatrix> &U ){
    QDPIO::cout << "Entered Solver::Solver" << std::endl;
    QDPIO::cout << "Solver::Solver Creating state..." << std::endl;
    State state = S.state(U);
    QDPIO::cout << "Solver::Solver state created; constructing solver" << std::endl;
    D_inv = S.solver(state, solver_parameters);
};

void Solver::operator()(LatticePropagator& result, const LatticePropagator& source){
    LatticeFermion chi,psi;
    for(unsigned int c=0; c < Nc; c++){
        for(unsigned int s=0; s < Ns; s++){
            chi=zero;
            psi=zero;
            Chroma::PropToFerm(source, chi, c, s);
            (*D_inv)(psi, chi);
            Chroma::FermToProp(psi, result, c, s);
        }
    }
}