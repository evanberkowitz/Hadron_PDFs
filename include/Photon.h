#ifndef _HADRON_PDF_PHOTON
#define _HADRON_PDF_PHOTON

LatticeComplex phases(const multi1d<int> &momentum){
    LatticeReal argument = zero;
    Real two_pi = Chroma::twopi;
    Real norm = 1.0;

    for_direction(d){
        // Calculate p.x:
        argument += LatticeReal((two_pi * Layout::latticeCoordinate(d) * momentum[d]) / Layout::lattSize()[d]);
        norm     *= Real(Layout::lattSize()[d]);
    }

    LatticeComplex b = zero;

    b = cmplx( cos(argument), sin(argument) ) ; // / norm;

    return b;
}


#endif /* end of include guard: _HADRON_PDF_PHOTON */