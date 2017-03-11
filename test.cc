#include "standard_includes.h"
#include "version.h"

using namespace Chroma;
    
int main(int argc, char **argv){
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    return 0;
}