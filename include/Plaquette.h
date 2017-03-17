#ifndef _HADRON_PDFS_PLAQUETTE
#define _HADRON_PDFS_PLAQUETTE

#include "include/ParallelTransporter.h"

// It is much faster to call, eg. Chroma::MesPlq.
// However, I implement this here to show that ParallelTransporter is doing the right thing.
// A plaquette is the trace of a square.  We can transport the unit matrix around the loop
// and sum the real part of the trace.

Double Plaquette(const multi1d<LatticeColorMatrix> &U){
        Double plq = 0.0;
        Real one=1.0;
        LatticeColorMatrix ONE=one;
        ParallelTransporter XY(U, "0:1;1:1;0:-1;1:-1;");
        ParallelTransporter XZ(U, "0:1;2:1;0:-1;2:-1;");
        ParallelTransporter XT(U, "0:1;3:1;0:-1;3:-1;");
        ParallelTransporter YZ(U, "1:1;2:1;1:-1;2:-1;");
        ParallelTransporter YT(U, "1:1;3:1;1:-1;3:-1;");
        ParallelTransporter ZT(U, "2:1;3:1;2:-1;3:-1;");
        
        plq+=sum(real(trace(XY(ONE))));
        plq+=sum(real(trace(XZ(ONE))));
        plq+=sum(real(trace(XT(ONE))));
        plq+=sum(real(trace(YZ(ONE))));
        plq+=sum(real(trace(YT(ONE))));
        plq+=sum(real(trace(ZT(ONE))));
        
        // Normalize:
        plq *= 2.0 / double(Layout::vol()*Nd*(Nd-1)*Nc);
        
        return plq;
    }
    



#endif /* end of include guard: _HADRON_PDFS_PLAQUETTE */
