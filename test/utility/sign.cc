#include "version.h"
#include "standard_includes.h"
#include <random>
#include <climits>

int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << banner("Test the sign function") << std::endl;
    std::uniform_int_distribution<> pos(1, INT_MAX);
    std::uniform_int_distribution<> neg(INT_MIN,-1);
    std::mt19937 generator;
    int N = 1000000;
    int temp;
    
    QDPIO::cout << "Testing positive integers... " << std::flush;
    bool pass=true;
    for(int i = 0; i<N; i++) {  
        temp = pos(generator);
        if (sign(temp) != 1) {
            QDPIO::cout << "sign fails on " << temp << ".  " << std::flush;
            pass=false;
            break;
        }
    }
    QDPIO::cout << (pass ? "PASS!" : "FAIL!") << std::endl;
    if( ! pass ){ QDP_abort(EXIT_FAILURE); }

    pass = ( sign(0) == 0 );
    QDPIO::cout << "Testing zero             ... " << ( pass ? "PASS!" : "FAIL!") << std::endl;
    if( ! pass ){ QDP_abort(EXIT_FAILURE); }

    QDPIO::cout << "Testing negative integers... " << std::flush;
    pass=true;
    for(int i = 0; i<N; i++) {  
        temp = neg(generator);
        if (sign(temp) != -1) {
            QDPIO::cout << "sign fails on " << temp << ".  " << std::flush;
            pass=false;
            break;
        }
    }
    QDPIO::cout << (pass ? "PASS!" : "FAIL!") << std::endl;
    if( ! pass ){ QDP_abort(EXIT_FAILURE); }
    
    Chroma::finalize();
    return 0;

}