#include "standard_includes.h"
#include <random>
#include <cfloat>
#include "include/Plaquette.h"
#include "include/ParallelTransporter.h"


int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 


    multi1d<int> lattice_size(4);
    lattice_size[0]=4;
    lattice_size[1]=4;
    lattice_size[2]=4;
    lattice_size[3]=32;
    Layout::setLattSize(lattice_size);
    Layout::create();
    RNG::initRNG();
    

    bool pass=true;
    double tol= 8 * DBL_EPSILON;

    QDPIO::cout << banner("Parallel Transporter") << std::endl;
    
    QDPIO::cout << "This serves as a test of the ParallelTransporter (and, inherently, the underlying WilsonLine) class." << std::endl;


    QDPIO::cout << banner("Plaquette") << std::endl;

    multi1d<LatticeColorMatrix> links(Nd);
    timed("Plaquettes test") for(int i = 0; i < 10; i++){
        LatticeReal dummy;
        random(dummy);
        QDPIO::cout << "We get a hot start gauge field..." << std::endl;
        Chroma::HotSt(links);   // Could also try Chroma::weakField(links), but it doesn't seem to work?
        Double plq = Plaquette(links);
        QDPIO::cout << "I compute the plaquette to be " << plq << std::endl;
        Double w=0, s=0, t=0, l=0;
        multi2d<Double> pp;
        Chroma::MesPlq(links, w, s, t, pp, l);
        QDPIO::cout << "Chroma::MesPlq finds          " << w << std::endl;

        double eps=0.0;
        QDPIO::cout << "    Agreement" << std::flush;
        for( eps = 1.0; abs(toDouble(plq - w)) < eps && eps > tol; eps/=2 ){
            QDPIO::cout << "." << std::flush;
        }
        QDPIO::cout << "\nDifferent by " << eps << " at least, tolerance is " << tol << std::endl;
        if( eps > tol ){
            QDPIO::cout << "FAIL!  This is too different." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
    }
    
    
    
    QDPIO::cout << banner("Non-zero shifts") << std::endl;
    QDPIO::cout << "Plaquettes are nice, but they have total displacement 0, so once we have the Wilson Line" << std::endl;
    QDPIO::cout << "nothing actually has to *move* anywhere.  We should make sure to test that things wind up where claimed" << std::endl;
    
    QDPIO::cout << "First, let's just push the unit matrix through one link." << std::endl;
    QDPIO::cout << "Creating the lattice of unit matrices..." << std::endl;
    Real one=1.0;
    LatticeColorMatrix ONE=one;
    
    for_direction(d){
        // QDPIO::cout << "Now, we can push it through the gauge field in the " << d << " direction..." << std::endl;
        std::string ds=int_to_string(d);
        ParallelTransporter push(links, ds+":1");
        LatticeColorMatrix pushed=push(ONE);
        Double ip=real(innerProduct(transpose(links[d]), shift(pushed,FORWARD,d)));
        double ip_norm=toDouble(ip)/double(Nc*Layout::vol()) ;
        // QDPIO::cout << "Normalizing by volume and Nc, one finds " << ip_norm << std::endl;
        if( abs(ip_norm - 1.0) > tol) {
            QDPIO::cout << "FAIL!  The global consistency check is too different."  << std::endl;
            QDP_abort(EXIT_FAILURE);
        } 
        // else{
        //     QDPIO::cout << "PASS!  They match to " << tol << " or better." << std::endl;
        // }
        // QDPIO::cout << "But, if there was some crazy bug, there could accidentally be a conspiracy." << std::endl;
        // QDPIO::cout << "Let's check site-by-site..." << std::endl;
        LatticeReal IP=1.0;
        LatticeInt  PASS=1;
    
        QDPIO::cout << "Direction " << d << " agrees to " << std::flush;
        double eps=0.0;
        int pass_count=Layout::vol();
        for( eps=1.0; pass_count == Layout::vol(); eps/=2){
            QDPIO::cout << "." << std::flush;
            IP=real(localInnerProduct(transpose(links[d]), shift(pushed, FORWARD, d)));
            PASS=where( fabs(IP-Nc) > eps, 0, 1 );
            pass_count=toInt(sum(PASS));
        }
        QDPIO::cout << eps << std::endl;
        if( eps > tol ){
            QDPIO::cout << "FAIL!  This is too different---tolerance is " << tol << std::endl;
            QDP_abort(EXIT_FAILURE);
        } 
    }
    

    QDPIO::cout << "PASS!  Tolerance is " << tol << std::endl;
    QDPIO::cout << "This demonstrates that one-site hops work." << std::endl;
    
    
    
    QDPIO::cout << banner("Multi-link shifts") << std::endl;
    
    QDPIO::cout << "Testing a multi-site shift is a bit trickier." << std::endl;
    QDPIO::cout << "One way is to do a two-step calculation of the plaquette." << std::endl;
    QDPIO::cout << "Another way is to move move a lattice color vector along a long-distance path, move it back, and compare." << std::endl;

    QDPIO::cout << banner("Multi-link shifts:  two-step plaquette") << std::endl;
    
    multi2d<ParallelTransporter*> HALF_PLAQ(6,2);
    
    LatticeColorMatrix halfway=zero, half_plq_temp=zero;
    timed("Two subsequent half-plaquettes test") for( int half_plaq_test_count = 0; half_plaq_test_count < 10; half_plaq_test_count++ ){
        
        HALF_PLAQ[0][0]=new ParallelTransporter(links, "0:1;1:1;");     HALF_PLAQ[0][1]=new ParallelTransporter(links, "0:-1;1:-1;");        //XY
        HALF_PLAQ[1][0]=new ParallelTransporter(links, "0:1;2:1;");     HALF_PLAQ[1][1]=new ParallelTransporter(links, "0:-1;2:-1;");        //XZ
        HALF_PLAQ[2][0]=new ParallelTransporter(links, "0:1;3:1;");     HALF_PLAQ[2][1]=new ParallelTransporter(links, "0:-1;3:-1;");        //XT
        HALF_PLAQ[3][0]=new ParallelTransporter(links, "1:1;2:1;");     HALF_PLAQ[3][1]=new ParallelTransporter(links, "1:-1;2:-1;");        //YZ
        HALF_PLAQ[4][0]=new ParallelTransporter(links, "1:1;3:1;");     HALF_PLAQ[4][1]=new ParallelTransporter(links, "1:-1;3:-1;");        //YT
        HALF_PLAQ[5][0]=new ParallelTransporter(links, "2:1;3:1;");     HALF_PLAQ[5][1]=new ParallelTransporter(links, "2:-1;3:-1;");        //ZT

        
        double plq=0.0;
        
        LatticeReal r=zero;
        LatticeColorMatrix start=zero;
        for( int plane=0; plane < 6; plane++ ){
            // On each site make a color matrix with diagonal entries that obey the gaussian distribution.
            gaussian(r);    start=r;
            // Transport that thing around.
            halfway             = (HALF_PLAQ[plane][0])->operator()(start);
            half_plq_temp       = (HALF_PLAQ[plane][1])->operator()(halfway);
            // On every site, divide by the entry before summing.
            plq                += toDouble(sum(real(trace(half_plq_temp) / r ) ));
            // I know this business of the random gaussian works, because when I change the ParallelTransporters above to be 
            // (the full plaquette) and (the zero path) I get the right answer to 1 part in 10^14, and if I overwrite the gaussian assignment with "r=1.0;" I get exact agreement.
            
            // It's not pointless to do this random thing---it's much more likely to pick up something "fishy" if it's wrong / if I'm not writing good tests.
        }
        
        // Normalize
        plq *= 2.0 / double(Layout::vol()*Nd*(Nd-1)*Nc);
        
        QDPIO::cout << "The two-step plaquette is " << plq << std::endl;
        Double w=0, s=0, t=0, l=0;
        multi2d<Double> pp;
        Chroma::MesPlq(links, w, s, t, pp, l);
        QDPIO::cout << "Chroma::MesPlq gives      " << w << std::endl;
        
        double eps=0.0;
        QDPIO::cout << "    Agreement" << std::flush;
        for( eps = 1.0; abs(toDouble(plq - w)) < eps && eps > tol; eps/=2 ){
            QDPIO::cout << "." << std::flush;
        }
        QDPIO::cout << "\nDifferent by " << eps << " at least, tolerance is " << tol << std::endl;
        if( eps > tol ){
            QDPIO::cout << "FAIL!  This is too different." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
        
        delete HALF_PLAQ[0][0];     delete HALF_PLAQ[0][1];        //XY
        delete HALF_PLAQ[1][0];     delete HALF_PLAQ[1][1];        //XZ
        delete HALF_PLAQ[2][0];     delete HALF_PLAQ[2][1];        //XT
        delete HALF_PLAQ[3][0];     delete HALF_PLAQ[3][1];        //YZ
        delete HALF_PLAQ[4][0];     delete HALF_PLAQ[4][1];        //YT
        delete HALF_PLAQ[5][0];     delete HALF_PLAQ[5][1];        //ZT
        
        Chroma::HotSt(links); // Update at the end, so that the first pass matches the previous case.
    }
    
    
    
    QDPIO::cout << banner("Multi-link shifts: a long-distance path and its reverse undo one another") << std::endl;
    
    
    multi2d<ParallelTransporter*> FWD_BWD(5,2);
    
    LatticeColorMatrix fwd=zero, bwd=zero;
    timed("Long-distance transport test") for( int fwd_bwd_test_count = 0; fwd_bwd_test_count < 10; fwd_bwd_test_count++ ){
        
        // Create parallel transporters.
        FWD_BWD[0][0]=new ParallelTransporter(links, "0:1;1:1;");                    FWD_BWD[0][1]=new ParallelTransporter(links, "1:-1;0:-1;");
        FWD_BWD[1][0]=new ParallelTransporter(links, "0:1;2:1;3:1;1:1");             FWD_BWD[1][1]=new ParallelTransporter(links, "1:-1;3:-1;2:-1;0:-1");
        FWD_BWD[2][0]=new ParallelTransporter(links, "0:-1;2:1;3:-1;1:1");           FWD_BWD[2][1]=new ParallelTransporter(links, "1:-1;3:+1;2:-1;0:+1");
        FWD_BWD[3][0]=new ParallelTransporter(links, "3:24");                        FWD_BWD[3][1]=new ParallelTransporter(links, "3:-24");
        FWD_BWD[4][0]=new ParallelTransporter(links, "0:4;1:7;3:1;3:-1;2:4");        FWD_BWD[4][1]=new ParallelTransporter(links, "2:-4;1:-7;0:-4");
        // FWD_BWD[4][0]=new ParallelTransporter(links, "1:1;3:1;");     FWD_BWD[4][1]=new ParallelTransporter(links, "3:-1;1:-1;");
        // FWD_BWD[5][0]=new ParallelTransporter(links, "2:1;3:1;");     FWD_BWD[5][1]=new ParallelTransporter(links, "3:-1;2:-1;");
        
        LatticeInt  PASS=1;
        int pass_count=0;
        
        for(int path=0; path < FWD_BWD.nrows(); path++){
            LatticeReal r=zero;
            LatticeColorMatrix start=zero;
            // On each site make a color matrix with diagonal entries that obey the gaussian distribution.
            gaussian(r);    start=r;
            // Transport that thing there and back.
            fwd = (FWD_BWD[path][0])->operator()(start);
            bwd = (FWD_BWD[path][1])->operator()(fwd);

            double eps=1.0;
            // Now, if the two really undo one another, if I subtract and normalize to (Nc*r) --- Nc because I trace.
            PASS=where( fabs(real(traceColor(bwd - start))/(Nc*r)) < 100, 1, 0);
            pass_count=toInt(sum(PASS));
            
            QDPIO::cout << "On cfg " << fwd_bwd_test_count << " path " << path << " shows agreement to" << std::flush;
            for( eps = 1.0;  pass_count == Layout::vol(); eps/=2 ){ //pass_count == Layout::vol() &&
                // QDPIO::cout << " eps: " << eps << " pass: " << pass_count << std::flush;
                QDPIO::cout << "." << std::flush;
                PASS=where( fabs(real(traceColor(bwd - start))/(Nc*r)) < eps, 1, 0);
                pass_count=toInt(sum(PASS));
            }
            QDPIO::cout << eps << std::endl;
            if( eps > tol){
                QDPIO::cout << "\nFAIL!  Tolerance is " << tol << std::endl;
                QDP_abort(EXIT_FAILURE);
            }
        }
        
        // Delete pointers.
        for(int i=0; i < FWD_BWD.nrows(); i++){
            delete FWD_BWD[i][0];   delete FWD_BWD[i][1];
        }
        
        Chroma::HotSt(links); // Update at the end
    }
    QDPIO::cout << "PASS!  Everything returned to its original value within tolerance of " << tol << std::endl;
    
    
    QDPIO::cout << banner("PASS!") << std::endl;
    
    Chroma::finalize();
    return 0;

}