#include "standard_includes.h"

int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    QDPIO::cout << "Above, you should see a banner describing the conditions under which you compiled this executable." << std::endl;
    QDPIO::cout << "It should describe the git branch and commit it was compiled against, and time of the commit." << std::endl;
    QDPIO::cout << "It should also report the compilation time of the executable." << std::endl;
    
    QDPIO::cout << "\n\n" << std::endl;
    QDPIO::cout << "If the git repo wasn't clean at the time of compilation, you will be warned." << std::endl;
    QDPIO::cout << "That way, you know if your version corresponds to a committed state, or if it possibly corresponds" << std::endl;
    QDPIO::cout << "to some modified version (during development, for example)." << std::endl;
    
    Chroma::finalize();
    return 0;

}