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
    multi1d<LatticeColorMatrix> transformed(Nd);
    LatticeColorMatrix          g, cg, A, B, tmp;

    Real one=1.0;
    LatticeColorMatrix ONE=one;

    timed("Plaquettes test") {
        LatticeReal dummy;
        random(dummy);
        QDPIO::cout << "We get a hot start gauge field..." << std::endl;
        Chroma::HotSt(links);   // Could also try Chroma::weakField(links), but it doesn't seem to work?

        transformed = links;
        rgauge(transformed, g);
        cg = conj(g);

        ParallelTransporter path(links, "0:1"); //";2:1;3:1;1:1");
        ParallelTransporter path_prime(transformed, "0:1"); //";2:1;3:1;1:1");
        A = path(ONE);
        B = path_prime(ONE);
        
        tmp = shift(g, -1, 0) * A * adj(g);
        A = tmp;
        
        Double ip=real(innerProduct(A, B));
        double ip_norm=toDouble(ip)/double(Nc*Layout::vol()) ;
        // QDPIO::cout << "Normalizing by volume and Nc, one finds " << ip_norm << std::endl;
        if( abs(ip_norm - 1.0) > tol) {
            QDPIO::cout << "FAIL!  The global consistency check is too different."  << std::endl;
            QDP_abort(EXIT_FAILURE);
        } 
        
    }
    QDPIO::cout << "PASS!  Everything returned to its original value within tolerance of " << tol << std::endl;
    
    
    QDPIO::cout << banner("PASS!") << std::endl;
    
    Chroma::finalize();
    return 0;

}