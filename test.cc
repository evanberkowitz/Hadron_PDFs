#include "standard_includes.h"
#include "version.h"

using namespace Chroma;
    
int main(int argc, char **argv){
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << "Testing for_direction macro: ";
    for_direction(dir) QDPIO::cout << dir << " ";
    return 0;
}