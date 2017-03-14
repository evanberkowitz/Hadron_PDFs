#include "standard_includes.h"

int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << "Test the for_direction macro." << std::endl;
    
    QDPIO::cout << "Directions are integers that range from 0 to " << Nd << std::endl;
    for_direction(d) QDPIO::cout << d << " ";

    return 0;
}