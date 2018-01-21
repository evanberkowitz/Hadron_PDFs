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
    // Therefore, if we want result(x+µ) = input(x), we need a negative QDP++ shift,
    // This is exactly the opposite of what *I* mean by a shift.

    // Consider this code from Chroma::MesPlq for computing the plaquette:
    //
    // sum(real(trace(u[mu]*shift(u[nu],FORWARD,mu)*adj(shift(u[mu],FORWARD,nu))*adj(u[nu]))))
    //
    // Here is a picture of the APPARENT Chroma convention corresponding to the above code.
    //
    //        x + nu ----- U[mu](x+nu) ---> x + nu + mu
    //          ^                               ^
    //          |                               |
    //          |                               |
    //          |                               |
    //       U[nu](x)                       U[nu](x+mu)
    //          |                               |
    //          |                               |
    //          |                               |
    //          |                               |
    //          x    ------ U[mu](x) -------> x + mu
    //
    // One can see, reading the code from left to right, that the plaquette goes counter clockwise.
    // This means that the left color index of U[mu](x) lives at x and the right color index lives at x+µ.
    //
    // So, if you have a color vector at x and you want to move it to x+mu, you need to multiply it by transpose(U[mu]) and then shift.
    // If you have something at x and you want to move it to x-mu, you shift it first and then multiply by conjugate(U[mu]).
    //
    // That would lead to a result
    //
    // return sign > 0 ? shift( transpose(U[dir]) * v, -1, dir) : conj(U[dir]) * shift(v, +1, dir);
    //
    // However, the above code is EXTREMELY MISLEADING!  
    // The issue, is that the real part of the trace of the plaquette is equal to the trace of the dagger of the plaquette:
    //
    // Chroma::MesPlq == sum(real(trace(u[nu] * shift(u[mu], FORWARD, nu) * adj(shift(u[nu], FORWARD, mu)) * adj(u[mu]))))
    //
    // This leads to a picture:
    //
    //        x + nu <---- U[mu](x+nu) ---- x + nu + mu
    //          |                               |
    //          |                               |
    //          |                               |
    //          |                               |
    //       U[nu](x)                       U[nu](x+mu)
    //          |                               |
    //          |                               |
    //          |                               |
    //          v                               v
    //          x    <----- U[mu](x) -------- x + mu
    //
    // so that if you have a color vector at x and you want to move it to x+mu you need to multiply it by adj(U[mu]) and then shift.
    // If you have something at x and you want to move it to x-mu, you shift it first and then multiply U[mu].
    //
    // Using this convention, we have 
    return sign > 0 ? shift( adj(U[dir]) * v, -1, dir) : U[dir] * shift(v, +1, dir);
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