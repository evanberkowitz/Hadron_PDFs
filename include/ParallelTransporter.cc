#include "standard_includes.h"
#include "include/ParallelTransporter.h"

// Parallel transport moves something from x+∆ to x, using the gauge links to move from site to site.
// We already have the WilsonLine that, given a path through the lattice, produces the correct product of the gauge links
// and stores that product at the destination.  Now we just need to shift our own data there and then multiply.

// QDP++ allows us to define macroscopic shifts, which is much cheaper than performing repeated single-link shifts.
// It requires a class that inherits from MapFunc.
class jump : public MapFunc {
public:
    jump(const multi1d<int> &displacement) : displacement(displacement) {};
    
    // That class needs the application operation that takes a multi1d representing the lattice location
    // and produces the coordinates where data that lands at that location should originate from.
    // Note that this exactly what we want---we have specified where the data we wind up with should have originated.
    // That way, data that lands at 0 will have started at displacement.
    multi1d<int> operator()( const multi1d<int> &x, int sign=1 ) const {
        multi1d<int> temp(Nd);
        int dim=0;
        for(int d = 0 ; d < Nd; d++){
            temp[d]=x[d] + sign*displacement[d];            // Add the displacement
            dim = Layout::lattSize()[d];                    // Make sure we land inside the lattice by
            while( temp[d] <=  0  ){ temp[d] += dim ;}      //     adding to negative coordinates
            temp[d]=temp[d] % dim;                          // and modding out by the dimension.
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

    for_direction(d){                          // In principle, more than one direction may be antiperiodic---
        if (periodicity[d] == 1) continue;     // we can ignore the periodic directions
        //
        //  Here's a schematic to understand where the signs are needed.  Assume a positive shift s (that is, s > 0):
        if( displacement[d] > 0){
        //  coordinate original     positive shift      antiperiodic
        //
        //  lattSize-1 +                                -       \______ These sites have (lattSize-1) - latticeCoordinate < s.
        //             +                                -       /       The last site has LHS = 0.  The second to last site has LHS=1.
        //  ...        +            +                   +               The LHS is nonnegative, so if s=0 no sign is applied.
        //             +            +                   +               
        //  2          +            +                   +               
        //  1          +            +                   +               
        //  0          +            +                   +               
        //                          +                                   
        //                          +                                   
            copy = where ( Layout::lattSize()[d] - 1 - Layout::latticeCoordinate(d) < displacement[d], (-1)*result, result) ;
            
        } else {
        //  Now let's think about a negative shift s (that is, s < 0):
        //
        //  coordinate original     negative shift      antiperiodic
        //
        //                          +                                   
        //                          +                                   
        //  lattSize-1 +            +                   +           ___ These sites have latticeCoordinate < -s.
        //             +            +                   +          |    The first site has LHS = 0.  The second LHS=1.
        //  ...        +            +                   +          |    The LHS is nonnegative, so if s=0 no sign is applied.
        //             +            +                   +          |    
        //  2          +            +                   +          |    
        //  1          +                                -       \__|    
        //  0          +                                -       /       
            copy = where ( Layout::latticeCoordinate(d) < -displacement[d], (-1)*result, result);
        }
        //
        //  There may be a beutiful expression to capture both the positive and negative s cases.

        // TODO: what about the annoying case when the displacement wraps an antiperiodic direction more than once?
        result = copy;
    }
    return result;
}

ParallelTransporter::ParallelTransporter(){
    WL = 1;
    displacement.resize(Nd);
    for_direction(d) displacement[d] = 0;
    shifter.make(jump(displacement));
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

ParallelTransporter::ParallelTransporter(const LatticeColorMatrix &product_of_links, const multi1d<int> &disp){
    WL = product_of_links;
    displacement.resize(Nd);
    for_direction(d) displacement[d] = disp[d];
    shifter.make(jump(displacement));
}

ParallelTransporter::ParallelTransporter(const ParallelTransporter &p){
    WL = p.WL;
    displacement.resize(Nd);
    for_direction(d) displacement[d] = p.displacement[d];
    shifter.make(jump(displacement));
}


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
    LatticeColorMatrix temp = WL*shifter(M);
    LatticeColorMatrix result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}


LatticeFermion      ParallelTransporter::operator()(const LatticeFermion        &F, bool antiperiodic){
    LatticeFermion temp = WL*shifter(F);
    LatticeFermion result = antiperiodic ? enforce_boundary_conditions(temp) : WL*shifter(temp);
    return result;
}

LatticeSpinMatrix   ParallelTransporter::operator()(const LatticeSpinMatrix     &S, bool antiperiodic){
    LatticeSpinMatrix temp = shifter(S);
    LatticeSpinMatrix result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}

LatticePropagator   ParallelTransporter::operator()(const LatticePropagator     &S, bool antiperiodic){
    LatticePropagator temp = WL*shifter(S);
    LatticePropagator result = antiperiodic ? enforce_boundary_conditions(temp) : temp;
    return result;
}

void ParallelTransporter::write(HDF5Writer &H5, std::string path){
    H5.write(path+"/displacement", displacement, HDF5Base::trunc);
    H5.write(path+"/WL",           WL,           HDF5Base::trunc);
}

ParallelTransporter ParallelTransporter::reverse(){
    multi1d<int> backward(Nd);
    for_direction(d)    backward[d] = -displacement[d];
    ParallelTransporter rev( adj(WL), backward );
    return rev;
}