#ifndef _WILSONLINE_H
#define _WILSONLINE_H

#include "standard_includes.h"
#include "include/Path.h"

// A Wilson Line is a path of gauge links all multiplied together that parallel-transports a color vector along a path.
//
// WL(x, dx_1, dx_2, ..., dx_n) = U(x) * U(x + dx1) * U( x + ... + dx_n)
//
// so that given color vector psi(x+dx), one can transport it to x by WL(x, dx...)*shift(psi(x+dx)).

class WilsonLine : public Path {
public:
    WilsonLine(const Path &P) : Path(P) {};
    WilsonLine(const multi2d<int> &P) : Path(P) {};
    WilsonLine(const std::string &P) : Path(P) {};

    LatticeColorMatrix operator()(const multi1d<LatticeColorMatrix> &U);

};


#endif /* end of include guard: _WILSONLINE_H */
