#include "standard_includes.h"

int main(int argc, char **argv){

    StopWatch swatch_everything;
    swatch_everything.reset();
	swatch_everything.start();
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << " We don't have enough infrastructure to build the measurement yet, but I thought I would lay out the pseudocode." << std::endl;
    QDPIO::cout << " Currently I describe the isospin limit, and consider light quarks only." << std::endl;
    
    QDPIO::cout << " For each configuration" << std::endl;
    {
    QDPIO::cout << "     For each source position" << std::endl;
        {
    QDPIO::cout << "         Get a source-to-all propagator S." << std::endl;
    QDPIO::cout << "         Build the pion correlator, as usual.  Fourier transform." << std::endl;
    QDPIO::cout << "         Build the nucleon correlator, as usual.  Fourier transform." << std::endl;
    QDPIO::cout << "         Write out the correlators and the propagator (only keep the current propagator)." << std::endl;

    QDPIO::cout << "         Do in a checkpointed way:" << std::endl;
    QDPIO::cout << "         For each incoming photon (momentum and spinor index)" << std::endl;
    QDPIO::cout << "         For each path you want to parallel transport your quark along" << std::endl;
    QDPIO::cout << "         For each outgoing photon (momentum and spinor index)" << std::endl;
            {
    QDPIO::cout << "             Apply the incoming photon vertex to the propagator." << std::endl;
    QDPIO::cout << "             Parallel transport the propagator along the path." << std::endl;
    QDPIO::cout << "             Apply the outgoing photon vertex to the propagator." << std::endl;
    QDPIO::cout << "             Re-solve the Dirac equation using that propagator as the source." << std::endl;
    QDPIO::cout << "             Use that propagator (and the original propagator) to build the pion and nucleon correlators.  Fourier transform." << std::endl;
    QDPIO::cout << "             This yields full-volume correlators, " << std::endl;
    QDPIO::cout << "                  <   destroyed with every momentum (p)" << std::endl;
    QDPIO::cout << "                      hard outgoing photon with definite momentum" << std::endl;
    QDPIO::cout << "                      Wilson line" << std::endl;
    QDPIO::cout << "                      hard ingoing photon with definite momentum" << std::endl;
    QDPIO::cout << "                      created at point >" << std::endl;
    QDPIO::cout << "             For each sink-side momentum, we know the source-side momentum by conservation." << std::endl;
    QDPIO::cout << "             Write out the full (momentum-)volume correlator." << std::endl;
    QDPIO::cout << "             Do analysis offline." << std::endl;
            }
        }
    }
    
    QDPIO::cout << banner("Finalize") << std::endl;
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
    
    swatch_everything.stop();
    QDPIO::cout << "Everything took " << swatch_everything.getTimeInSeconds() << " seconds." << std::endl;
    
    return 0;
}