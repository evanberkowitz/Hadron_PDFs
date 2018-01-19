#include "Hadron_PDFs.h"
#include "standard_includes.h"


int main(int argc, char **argv){
    
    Hadron_PDFs::initialize(&argc, &argv);
    
    QDPIO::cout << "Test the for_direction macro." << std::endl;
    
    QDPIO::cout << "Directions are integers that range from 0 to " << Nd-1 << " inclusive." << std::endl;
    for_direction(d) QDPIO::cout << d << " ";

    int test=0; 
    for_direction(d) {
        if ( d != test ){
            QDPIO::cout << "FAIL: " << d << " not correct, should be " << test << "." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
        test++;
    }
    if (test != Nd){
        QDPIO::cout << "FAIL: Didn't get up to the last dimension " << Nd << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    
    QDPIO::cout << "PASS!" << std::endl;

    

    Chroma::finalize();
    return 0;

}