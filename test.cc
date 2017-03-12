#include "standard_includes.h"
#include "version.h"
#include "include/Path.h"
    
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
    
    QDPIO::cout << "Testing for_direction macro: ";
    for_direction(dir) QDPIO::cout << dir << " ";
    QDPIO::cout << std::endl;
    
    
    QDPIO::cout << "Testing Path utilities " << std::endl;
    multi2d<int> test_path_0(10,Nd), test_path_1(10, Nd);
    for( int r=0; r < test_path_0.nrows(); r++ ){
        for_direction(d) {
            test_path_0[r][d]=0;
            test_path_1[r][d]=0;
        }
    }
    test_path_0[0][0]=1;    test_path_1[0][0]=1; 
    test_path_0[1][0]=1;    test_path_1[1][0]=1; 
    test_path_0[2][0]=3;    test_path_1[2][3]=3; 
    test_path_0[3][0]=-5;   test_path_1[3][3]=-5;
    test_path_0[4][0]=2;    test_path_1[4][3]=2; 
    test_path_0[5][0]=-1;   test_path_1[5][0]=-1;
    test_path_0[6][0]=1;    test_path_1[6][2]=10;
    test_path_0[7][0]=-2;   test_path_1[7][1]=3; 
    test_path_0[8][0]=3;    test_path_1[8][3]=3; 
    test_path_0[9][0]=-3;   test_path_1[9][3]=-5;
    
    QDPIO::cout << "Constructing P0..." << std::endl;   Path P0(test_path_0);
    QDPIO::cout << "Constructing P1..." << std::endl;   Path P1(test_path_1);
    QDPIO::cout << "done!" << std::endl;
    
    QDPIO::cout << "P0 is secretly the zero path:" << std::endl;
    QDPIO::cout << "Short:" << P0.short_string() << std::endl;
    QDPIO::cout << "Long :" << P0.long_string()  << std::endl;
    
    QDPIO::cout << "P1 is x^1 z^10 y^3 t^(-2)." << std::endl;
    QDPIO::cout << "Short:" << P1.short_string() << std::endl;
    QDPIO::cout << "Long :" << P1.long_string()  << std::endl;
    
    QDPIO::cout << "Finalizing..." << std::flush;
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
    
    return 0;
}