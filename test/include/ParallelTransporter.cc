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
    lattice_size[3]=8;
    Layout::setLattSize(lattice_size);
    Layout::create();
    RNG::initRNG();
    

    bool pass=true;

    QDPIO::cout << banner("Parallel Transporter") << std::endl;
    
    QDPIO::cout << "This serves as a test of the ParallelTransporter (and, inherently, the underlying WilsonLine) class." << std::endl;


    QDPIO::cout << banner("Plaquette") << std::endl;

    multi1d<LatticeColorMatrix> links(Nd);
    for(int i = 0; i < 10; i++){
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

        double eps=0.0, tol= 2 * DBL_EPSILON;
        QDPIO::cout << "    Agreement" << std::flush;
        for( eps = 1.0; abs(toDouble(plq - w)) < eps && eps > tol; eps/=16 ){
            QDPIO::cout << "." << std::flush;
        }
        QDPIO::cout << "\nDifferent by " << eps << " at most, tolerance is " << tol << std::endl;
        if( eps > tol ){
            QDPIO::cout << "FAIL!  This is too different." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
    }
    
    QDPIO::cout << banner("Non-zero shifts") << std::endl;
    QDPIO::cout << "Plaquettes are nice, but they have total displacement 0, so once we have the Wilson Line" << std::endl;
    QDPIO::cout << "nothing actually has to *move* anywhere.  We should make sure to test that things wind up where claimed" << std::endl;
    

    QDPIO::cout << banner("PASS!") << std::endl;
    
    Chroma::finalize();
    return 0;

}