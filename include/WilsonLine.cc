#include "standard_includes.h"
#include "include/Path.h"
#include "include/WilsonLine.h"


// 
// We want to compute W(x <--path-- x+displacement) = U[x] * U[x+dx1] * U[x + dx1 + dx2] * ...
//
// and store the result at x.
//
// This can be thought of as a fold across the steps.
// The partial result can be thought of as repeatedly applying a gauge shift,
//
// result( x ) = U(x)^µ * input(x + µ)
//
LatticeColorMatrix gauge_shift( const multi1d<LatticeColorMatrix> &U, const LatticeColorMatrix &v, const int sign, const int dir){
    // It's clear we need to shift the data around.
    //
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

    // Consider this code from Chroma::MesPlq for computing the plaquette:
    //
    // sum(real(trace(u[mu]*shift(u[nu],FORWARD,mu)*adj(shift(u[mu],FORWARD,nu))*adj(u[nu]))))
    //
    // Here is a picture of the Chroma convention, computing the plaquette
    //
    //        x + nu <----- U[mu](x+nu) --- x + nu + mu
    //          |                               |
    //          |             -->--             |
    //          |           /       \           |
    //          |          ^  CLOCK  |          |
    //       U[nu](x)      |  WISE!  v      U[nu](x+mu)
    //          |           \       /           |
    //          |             --<--             |
    //          |                               |
    //          v                               v
    //          x    <----- U[mu](x) -------- x + mu
    //
    // So, if you have a color vector at x+mu and you want to move it to x, you need to shift and then multiply by U[mu].
    // If you have a color vector at x-mu and want to move it to x, you need to multiply by adj(U[mu]) and then shift.
    //
    // We'll actually be shifting color matrices around, but the "other" color index is a color index that always lives
    // at the beginning of the path, and so shouldn't be transformed.
    //
    return sign > 0 ? U[dir]*shift( v, FORWARD, dir) : shift( adj(U[dir])*v, BACKWARD, dir);
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