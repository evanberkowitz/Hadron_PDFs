#include "standard_includes.h"
#include "include/ParallelTransporter.h"

// Parallel transport moves something from x to x + ∆, using the gauge links to move from site to site.
// We already have the WilsonLine that, given a path through the lattice, produces the correct product of the gauge links
// and stores that product at the destination.  Now we just need to shift our own data there and then multiply.

// QDP++ allows us to define macroscopic shifts, which is much cheaper than performing repeated single-links shifts.
// It requires a class that inherits from MapFunc.
class jump : public MapFunc {
public:
    jump(const multi1d<int> &displacement) : displacement(displacement) {};
    
    // That class needs the application operation that takes a multi1d representing the lattice location
    multi1d<int> operator()( const multi1d<int> &x, int sign=1 ) const {
        // and produces the coordinates where data that lands at that location should originate from.
        multi1d<int> temp(Nd);
        for(int d = 0 ; d < Nd; d++){
        // Note that this is the opposite of what we want---we have specified where the data we have should go.
        // Therefore, just as in WilsonLine.cc we need the opposite sign.  This way data at 0 will go to displacement,
        // rather than vice-versa.
        temp[d]=x[d] - sign*displacement[d];
        while( temp[d] <= 0 ){ temp[d] += Layout::lattSize()[d] ;}
        temp[d]=temp[d] % Layout::lattSize()[d];
        }
        return temp;
    };
    
    multi1d<int> displacement;
};

// If a quantity gets shifted across an antiperiodic boundary, we need to apply a bunch of minus signs.
template<class T> OLattice<T> ParallelTransporter::enforce_boundary_conditions(const OLattice<T>& field){

    // Here we specify the periodicity of the lattice.
    multi1d<int> periodicity(Nd);
    // TODO: this hard-codes the boundary conditions.  It should be set programmatically if possible.
    for_direction(d) periodicity[d]=1;  // We assume it's periodic in every direction
    periodicity[Nd-1]=-1;               // except for the last direction, which we assume is the time direction.
    
    OLattice<T> copy, result=field;

    for_direction(d){                                                                              // In principle, more than one direction may be antiperiodic---
        if (periodicity[d] == 1) continue;                                                         // we can ignore the periodic directions
        copy = where ( displacement[d] > Layout::latticeCoordinate(d) , (-1)*result, result);      // and flip the sign of everyplace that wrapped around.1
        // TODO: what if the displacement wraps an antiperiodic direction more than once?
        // With 2 seconds of thought I conjecture something like
        // int wrap = displacement / Layout::latticeSize(d);        // is Layoute::latticeSize a function?
        // copy = where ( displacement > Layout::latticeCoordinate(d) , (-1)^(wrap+1)*result, (-1)^wrap * result);
        // but it deserves deeper thought.  Anyway, you shouldn't pick paths like that.  If you do you may be punished.
        result = copy;
    }
    return result;
}

ParallelTransporter::ParallelTransporter(const multi1d<LatticeColorMatrix> &U, const Path &P) : 
    WL(WilsonLine(P)(U)),
    displacement(P.total_displacement())
{
    shifter.make(jump(displacement));
};

ParallelTransporter::ParallelTransporter(const multi1d<LatticeColorMatrix> &U, const std::string P)
{
    Path temp(P);
    WilsonLine W(P);
    WL=W(U);
    displacement.resize(Nd);
    for_direction(d) { displacement[d] = temp.total_displacement()[d]; }
    shifter.make(jump(displacement));
};


LatticeReal         ParallelTransporter::operator()(const LatticeReal           &R, bool antiperiodic){
    LatticeReal result = antiperiodic ? enforce_boundary_conditions(shifter(R)) : shifter(R);
    return result;
}

LatticeComplex      ParallelTransporter::operator()(const LatticeComplex        &C, bool antiperiodic){
    LatticeComplex result = antiperiodic ? enforce_boundary_conditions(shifter(C)) : shifter(C);
    return result;
}

LatticeColorVector  ParallelTransporter::operator()(const LatticeColorVector    &V, bool antiperiodic){
    LatticeColorVector temp = WL*shifter(V);
    LatticeColorVector result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}

LatticeColorMatrix  ParallelTransporter::operator()(const LatticeColorMatrix    &M, bool antiperiodic){
    LatticeColorMatrix temp = shifter(M)*WL;
    LatticeColorMatrix result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}


LatticeFermion      ParallelTransporter::operator()(const LatticeFermion        &F, bool antiperiodic){
    LatticeFermion temp = WL*shifter(F);
    LatticeFermion result = antiperiodic ? enforce_boundary_conditions(temp) : WL*shifter(temp);
    return result;
}

LatticePropagator   ParallelTransporter::operator()(const LatticePropagator     &S, bool antiperiodic){
    LatticePropagator temp = WL*shifter(S);
    LatticePropagator result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}