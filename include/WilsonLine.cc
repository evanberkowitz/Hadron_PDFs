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

    // Consider this code from Chroma::MesPlq for computing the plaquette:
    //
    // sum(real(trace(u[mu]*shift(u[nu],FORWARD,mu)*adj(shift(u[mu],FORWARD,nu))*adj(u[nu]))))
    //
    // Here is a picture:
    //
    //        x + nu ------ U[mu](x+nu) --> x + nu + mu
    //          ^                               ^
    //          |                               |
    //          |                               |
    //          |                               |
    //       U[nu](x)                       U[nu](x+mu)
    //          |                               |
    //          |                               |
    //          |                               |
    //          |                               |
    //          x    ------- U[mu](x) ------> x + mu
    //
    // Now, consider the multiplication of u[mu] * shift(u[nu], FORWARD, mu).
    // u[mu] is the link on the bottom of my picture.  In must have gauge indices at x and x+mu.
    // shift(u[nu], FORWARD, mu) is the link on the right side of my picture.
    // It must have gauge indices at x + mu and x + mu + nu.
    // In order to perform a sensible operation from the gauge-group point of view, we can only multiply indices that live on the same site!
    // So, that means the RIGHT index of u[mu] lives at x + mu, because it is multiplied on the right by u[nu](x+mu).
    // So, the "here" index is the left index, and the "there" index is the right index,
    //
    // EXACTLY THE OPPOSITE OF HOW YOU WOULD WRITE IN PHYSICS:  PSI(x+mu) = U[mu](x) * PSI(x)
    //
    // Since the indices are backwards, we need to multiply like (vector) * (link) rather than (link) * (vector).
    //
    // Shifting in the positive direction isn't so bad.
    // v(x) * U[dir](x) has gauge indices as x+dir.  We want those indices to land at x+dir, so we need shift( _, -1, dir).
    //
    // Shifting in the negative direction is a little subtle.
    // U[dir](x-mu) has gauge indices at x-mu and x.  But we want to backtrack that link, so we need adj(U).  
    // But adj has a transpose in it, so U adjoing has gauge indices at x and x-mu.  
    // The best way to compute this is to shift v to x-mu and then multiply by adj(U).
    // return sign > 0 ? shift( v * U[dir], -1, dir) : shift( v * adj(shift(U[dir], -1, dir)) , +1, dir);
    return sign > 0 ? shift( v * U[dir], -1, dir) : shift(v, +1, dir) * adj(U[dir]) ;
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