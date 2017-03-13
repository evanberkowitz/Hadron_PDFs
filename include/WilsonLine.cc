#include "standard_includes.h"
#include "include/Path.h"
#include "include/WilsonLine.h"


// We want to compute U(x + dx1+dx2+...+dxN) * ... * U(x + dx1 + dx2) * U(x + dx1) * 1(x)
// and store the result at x + dxN.

// This can be thought of as a fold across the steps.    
// The partial result can be thought of as repeatedly applying a gauge shift,
//
// result( x + µ ) = U(x)^µ * input(x)
//
LatticeColorMatrix gauge_shift( const multi1d<LatticeColorMatrix> &U, const LatticeColorMatrix &v, const int sign, const int dir){
    // so it's clear we need to shift the data around.
    // It's better to shift the partially-accumulated result around than the whole gauge field, 
    // because the result is just a lattice color matrix, while the gauge field is a Lorentz four-vector 
    // of lattice color matrices, so it would require Nd times as much communication.
    
    // We know that if sign=0 we don't have to do anything at all.
    if(sign == 0) return v;
    
    // In QDP++ shifts are a little confusing.  The QDP++ manual says that if
    //
    // r = shift(a, sign, dir)
    //
    // then r(x) = a(x + sign * \hat{dir} ).
    // Therefore, if we want result(x+µ) = U^µ(x) * v(x), we need a negative QDP++ shift,
    // result(x) = U^µ(x-µ) * v(x-µ)
    // This is exactly the opposite of what *I* mean by a shift.
    return sign > 0 ? shift(U[dir] * v, -1, dir) : shift(adj(shift(U[dir], -1, dir)) * v, +1, dir);
}

LatticeColorMatrix WilsonLine::operator()(const multi1d<LatticeColorMatrix> &U){
    START_CODE();
    
    // We start all the way on the right with the unit matrix
    Real one=1.0;
    LatticeColorMatrix result=one, temp=zero;

    for( int step=0; step< Path::dx.nrows(); step++ ){      // For each unit step 
        for_direction(d){                                   //
            int s=sign(dx[step][d]);                        //
            if( 0 == s ){ continue; }                       // find the direction it points
            temp    = gauge_shift(U, result, s, d);         // and do a gauge shift in that direction.
            result  = temp;                                 //
            break;                                          // stop once you found the right direction.
        }
    }
    
    END_CODE();
    return result;
}