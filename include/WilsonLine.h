#ifndef _WILSONLINE_H
#define _WILSONLINE_H

#include "standard_includes.h"
#include "include/Path.h"

// A Wilson Line is a path of gauge links all multiplied together that parallel-transports a color vector along a path.
//
// WL(x, dx_1, dx_2, ..., dx_n) = U(x + dx_1 + dx_2 + ... + dx_n) * ... * U(x + dx_1 + dx_2 ) * U(x + dx_1)
//
// so that given color vector psi(x), one can transport it to x + dx by WL(x, dx...)*psi(x).

class WilsonLine : public Path {
public:
    WilsonLine(const Path &P) : Path(P) {};
    WilsonLine(const multi2d<int> &P) : Path(P) {};
    WilsonLine(const std::string &P) : Path(P) {};
    // using Path::Path;
    LatticeColorMatrix operator()(const multi1d<LatticeColorMatrix> &U);

};


#endif /* end of include guard: _WILSONLINE_H */
