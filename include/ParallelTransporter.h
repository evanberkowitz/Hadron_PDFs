#ifndef _HADRON_PDF_PARALLEL_TRANSPORTER
#define _HADRON_PDF_PARALLEL_TRANSPORTER

#include "standard_includes.h"
#include "include/Path.h"
#include "include/WilsonLine.h"

#define ANTIPERIODIC true
#define PERIODIC     false

// A parallel transporter is a function
//
// lattice gauge field --> lattice object --> lattice object
//
// The result (for an object with color indices) is the gauge-link transported lattice color vector,
//
// psi( x + dx1 + dx2 + ... + dxN ) = U( x + dx1 + dx2 + ... + dxN ) ... U(x + dx1 + dx2) U(x + dx1) psi(x)

class ParallelTransporter {
public:
    ParallelTransporter(const multi1d<LatticeColorMatrix> &U, const Path &P);
    ParallelTransporter(const multi1d<LatticeColorMatrix> &U, const std::string P);
    LatticeReal         operator()(const LatticeReal        &R, bool antiperiodic=false);
    LatticeComplex      operator()(const LatticeComplex     &C, bool antiperiodic=false);
    LatticeColorVector  operator()(const LatticeColorVector &V, bool antiperiodic=false);
    LatticeColorMatrix  operator()(const LatticeColorMatrix &M, bool antiperiodic=false);
    LatticeFermion      operator()(const LatticeFermion     &F, bool antiperiodic=true );
    LatticePropagator   operator()(const LatticePropagator  &S, bool antiperiodic=true );
    multi1d<int> total_displacement(){ return displacement; };
private:
    template<class T> OLattice<T> enforce_boundary_conditions(const OLattice<T>& field);
    LatticeColorMatrix WL;
    multi1d<int> displacement;
    Map shifter;
};

#endif /* end of include guard: _HADRON_PDF_PARALLEL_TRANSPORTER */
