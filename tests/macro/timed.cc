#include "standard_includes.h"

int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << "Test the timed macro." << std::endl;
    
    QDPIO::cout << banner("A timed for loop:") << std::endl;
    timed("Whole loop") for(int i=0; i < 200; i++) QDPIO::cout << i << " ";
    
    QDPIO::cout << banner("A timed iteration:") << std::endl;
    for(int i=0; i<10; i++) timed("iteration "+int_to_string(i)) { QDPIO::cout << "iter" << std::endl; }
    
    QDPIO::cout << banner("Time a whole loop and every iteration:") << std::endl;
    timed("Whole loop") for(int i=0; i<10; i++) timed("iteration "+int_to_string(i)) { QDPIO::cout << "iter" << std::endl; }
    
    QDPIO::cout << banner("Time a whole loop and every iteration, which is itself a loop:") << std::endl;
    timed("Whole loop") for(int i=0; i<10; i++) timed("iteration "+int_to_string(i)) for(int j=0; j < 200; j++) QDPIO::cout << i << "." << j << " ";
}