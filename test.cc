#include "standard_includes.h"
#include "include/Path.h"


int main(int argc, char **argv){
    
    StopWatch swatch_everything;
    swatch_everything.reset();
	swatch_everything.start();
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 
    
    multi1d<int> lattice_size(4);
    lattice_size[0]=4;
    lattice_size[1]=4;
    lattice_size[2]=4;
    lattice_size[3]=8;
    Layout::setLattSize(lattice_size);
    Layout::create();
    
    QDPIO::cout << banner("Macros") << std::endl;
    
    QDPIO::cout << "Testing for_direction macro: ";
    for_direction(dir) QDPIO::cout << dir << " ";
    QDPIO::cout << std::endl;
    
    
    QDPIO::cout << banner("Testing Path") << std::endl;
    multi2d<int> test_path_0(10,Nd), test_path_1(10, Nd), test_path_2(10,Nd);
    for( int r=0; r < test_path_0.nrows(); r++ ){
        for_direction(d) {
            test_path_0[r][d]=0;
            test_path_1[r][d]=0;
            test_path_2[r][d]=0;
        }
    }
    test_path_0[0][0]=1;    test_path_1[0][0]=1;    test_path_2[0][0]=1; 
    test_path_0[1][0]=1;    test_path_1[1][0]=1;    test_path_2[1][1]=1; 
    test_path_0[2][2]=3;    test_path_1[2][3]=3;    test_path_2[2][2]=3; 
    test_path_0[3][2]=-5;   test_path_1[3][3]=-5;   test_path_2[3][3]=-5;
    test_path_0[4][3]=-1;    test_path_1[4][3]=2;    test_path_2[4][0]=-2; 
    test_path_0[5][3]=1;   test_path_1[5][0]=-1;   test_path_2[5][1]=1;
    test_path_0[6][2]=2;    test_path_1[6][2]=10;   test_path_2[6][0]=1; 
    test_path_0[7][0]=-2;   test_path_1[7][1]=3;    test_path_2[7][1]=-2;
    test_path_0[8][1]=-3;    test_path_1[8][3]=3;    test_path_2[8][3]=5; 
    test_path_0[9][1]=3;   test_path_1[9][3]=-5;   test_path_2[9][2]=-3;
    
    QDPIO::cout << "Constructing P0..." << std::flush;   Path P0(test_path_0);  QDPIO::cout << "done!" << std::endl;
    QDPIO::cout << "Constructing P1..." << std::flush;   Path P1(test_path_1);  QDPIO::cout << "done!" << std::endl;
    QDPIO::cout << "Constructing P2..." << std::flush;   Path P2(test_path_2);  QDPIO::cout << "done!" << std::endl;
    
    QDPIO::cout << "P0 is built from x x z^(3) z^(-5) t^(-1) t z^(2) x^(-2) y^(-3) y^3" << std::endl;
    if (! P0.is_nonzero()) QDPIO::cout << "P0 is secretly the zero path:" << std::endl;
    QDPIO::cout << "Short:" << P0.short_string() << std::endl;
    QDPIO::cout << "Long :" << P0.long_string()  << std::endl;
    QDPIO::cout << "Displacment: " ; for_direction(d) QDPIO::cout << P0.total_displacement()[d] << " "; QDPIO::cout << std::endl;
    
    QDPIO::cout << "P1 is x^1 z^10 y^3 t^(-2)." << std::endl;
    if (! P1.is_nonzero()) QDPIO::cout << "P1 is secretly the zero path:" << std::endl;
    QDPIO::cout << "Short:" << P1.short_string() << std::endl;
    QDPIO::cout << "Long :" << P1.long_string()  << std::endl;
    QDPIO::cout << "Displacment: " ; for_direction(d) QDPIO::cout << P1.total_displacement()[d] << " "; QDPIO::cout << std::endl;
    
    QDPIO::cout << "P2 is not the zero path, but it still has displacement zero:" << std::endl;
    QDPIO::cout << "It's x y z^3 t^(-5) x^(-2) y x y^(-2) t^5 z^(-3)" << std::endl;
    if (! P2.is_nonzero()) QDPIO::cout << "P2 is secretly the zero path:" << std::endl;
    QDPIO::cout << "Short:" << P2.short_string() << std::endl;
    QDPIO::cout << "Long :" << P2.long_string()  << std::endl;
    QDPIO::cout << "Displacment: " ; for_direction(d) QDPIO::cout << P2.total_displacement()[d] << " "; QDPIO::cout << std::endl;

    std::string test_path_3="0:3; 1: 2;2 :    1; 2:-2; 3:0 ";
    QDPIO::cout << "Constructing P3..." << std::flush;   Path P3(test_path_3);  QDPIO::cout << "done!" << std::endl;
    if (! P3.is_nonzero()) QDPIO::cout << "P3 is secretly the zero path:" << std::endl;
    QDPIO::cout << "P3 is built from " << test_path_3 << std::endl;
    QDPIO::cout << "Short:" << P3.short_string() << std::endl;
    QDPIO::cout << "Long :" << P3.long_string()  << std::endl;
    QDPIO::cout << "Displacment: " ; for_direction(d) QDPIO::cout << P3.total_displacement()[d] << " "; QDPIO::cout << std::endl;

    std::string test_path_4="0:3;1:2;2:1;2:-2;3:1;3:-1;3:0;2:1;3:5;3:-2;3:-3;1:-2;0:-3";
    QDPIO::cout << "Constructing P4..." << std::flush;   Path P4(test_path_4);  QDPIO::cout << "done!" << std::endl;
    QDPIO::cout << "P4 is built from " << test_path_4 << std::endl;
    if (! P4.is_nonzero()) QDPIO::cout << "P4 is secretly the zero path:" << std::endl;
    QDPIO::cout << "Short:" << P4.short_string() << std::endl;
    QDPIO::cout << "Long :" << P4.long_string()  << std::endl;
    QDPIO::cout << "Displacment: " ; for_direction(d) QDPIO::cout << P4.total_displacement()[d] << " "; QDPIO::cout << std::endl;

    
    QDPIO::cout << banner("Finalizing") << std::endl;
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
    
    swatch_everything.stop();
    QDPIO::cout << "Everything took " << swatch_everything.getTimeInSeconds() << " seconds." << std::endl;
    
    
    return 0;
}