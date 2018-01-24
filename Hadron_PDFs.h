#ifndef _HADRON_PDFS_H
#define _HADRON_PDFS_H

#include "version.h"
#include "standard_includes.h"
#include "io/parameters/lattice.h"

#include "actions/ferm/invert/syssolver_linop_cg.h"

bool linkageHack(){
    bool status=true;
    
    status &= InlineAggregateEnv::registerAll();
    status &= GaugeInitEnv::registerAll();
    status &= LinOpSysSolverCGEnv::registerAll();
    
    return status;
}

namespace Hadron_PDFs {

void initialize(int *argc, char ***argv){
    
    // Parse flags?
    
    // Initialize the chroma underlayer.
    Chroma::initialize(argc, argv);
    
    // Report on linkage hack.
    bool lH=linkageHack();
    
    QDPIO::cout << "Linkage Hack returned " << ( lH ? "true" : "false") << std::endl;
    
    // Install signal handlers here.
    // Use an #ifndef to include the appropriate header above,
    // and then un-comment the below
    #ifdef SIGNAL_HANDLERS
         signal_handlers::install();
    #endif
         
    // DEBUG_PRINT("DEBUG ENABLED");
         
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl;
    
    if( ! file_exists(Chroma::getXMLInputFileName()) ){
        QDPIO::cout << "Input file not found: " << Chroma::getXMLInputFileName() << std::endl;
        Chroma::finalize();
        exit(EXIT_FAILURE);
    }
    
    QDPIO::cout << "Input file:" << std::endl;
    QDPIO::cout << "        " << Chroma::getXMLInputFileName() << std::endl;
    
    XMLReader R;
    R.open(Chroma::getXMLInputFileName());
    
    lattice_parameters lattice;
    read(R, "/Hadron_PDFs/lattice", lattice);
    
    R.close();
    
    Layout::setLattSize(lattice.dimensions);
    Layout::create();

}

void finalize(){
    QDPIO::cout << banner("Finalize") << std::endl;
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
}

}


#endif /* end of include guard: _HADRON_PDFS_H */
