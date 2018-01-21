#include "version.h"
#include "standard_includes.h"
#include "include/Path.h"

int main(int argc, char **argv){
    
    Chroma::initialize(&argc, &argv);
    QDPIO::cout << Hadron_PDFs_compilation() << std::endl; 


    multi1d<int> lattice_size(4);
    lattice_size[0]=4;
    lattice_size[1]=4;
    lattice_size[2]=4;
    lattice_size[3]=8;
    Layout::setLattSize(lattice_size);
    Layout::create();

    bool pass=true;

    
    QDPIO::cout << banner("Testing Path") << std::endl;
    
    QDPIO::cout << "Creating a bunch of paths-creating inputs..." << std::endl;
    
    
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
    test_path_0[4][3]=-1;   test_path_1[4][3]=2;    test_path_2[4][0]=-2; 
    test_path_0[5][3]=1;    test_path_1[5][0]=-1;   test_path_2[5][1]=1;
    test_path_0[6][2]=2;    test_path_1[6][2]=10;   test_path_2[6][0]=1; 
    test_path_0[7][0]=-2;   test_path_1[7][1]=3;    test_path_2[7][1]=-2;
    test_path_0[8][1]=-3;   test_path_1[8][3]=3;    test_path_2[8][3]=5; 
    test_path_0[9][1]=3;    test_path_1[9][3]=-5;   test_path_2[9][2]=-3;
    
    std::string test_path_3="0:3; 1: 2;2 :    1; 2:-2; 3:0 ";
    std::string test_path_4="0:3;1:2;2:1;2:-2;3:1;3:-1;3:0;2:1;3:5;3:-2;3:-3;1:-2;0:-3";
    
    QDPIO::cout << "Creating paths..." << std::endl;
    
    int nP=5;
    multi1d<Path> P(nP);
    P[0] = Path(test_path_0);
    P[1] = Path(test_path_1);
    P[2] = Path(test_path_2);
    P[3] = Path(test_path_3);
    P[4] = Path(test_path_4);
    
    QDPIO::cout << banner("P[0]") << std::endl;
    QDPIO::cout << "P[0] is built from an array corresponding to x x z^(3) z^(-5) t^(-1) t z^(2) x^(-2) y^(-3) y^3" << std::endl;
    QDPIO::cout << "P[0] is a complicated way of writing the zero path." << std::endl;
    if ( P[0].is_nonzero() ){
        QDPIO::cout << "FAIL! That isn't reflected in P[0].is_nonzero()!" << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    for_direction(d) if( 0 != P[0].total_displacement()[d] ) {
        QDPIO::cout << "FAIL! Displacement in direction " << d << " is " << P[0].total_displacement()[d] << "." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    QDPIO::cout << "PASS!  The displacement is the zero vector!" << std::endl;
    
    
    
    
    QDPIO::cout << banner("P[1]") << std::endl;
    QDPIO::cout << "P[1] is built from an array corresponding to x x z^3 z^(-5) z^2 x^-1 z^10 y^3 t^3 t^-5" << std::endl;
    std::string P1_short="0:1;2:10;1:3;3:-2;";
    std::string P1_long ="0:1;2:1;2:1;2:1;2:1;2:1;2:1;2:1;2:1;2:1;2:1;1:1;1:1;1:1;3:-1;3:-1;";
    multi1d<int> P1_displacement(Nd);
    P1_displacement[0] = 1; P1_displacement[1] = 3; P1_displacement[2] = 10; P1_displacement[3] = -2;
    QDPIO::cout << "Manually, I find the short string to be " << P1_short << std::endl;
    pass= (P1_short == P[1].short_string());
    QDPIO::cout << ( pass ? "PASS!" : "FAIL!" ) << "                                   " << P[1].short_string() << std::endl;
    QDPIO::cout << "and the long string to be " << P1_long << std::endl;
    pass= (P1_long  == P[1].long_string() );
    QDPIO::cout << ( pass ? "PASS!" : "FAIL!" ) << "                     " << P[1].long_string()  << std::endl;

    for_direction(d) if ( P1_displacement[d] != P[1].total_displacement()[d] ){
        QDPIO::cout << "FAIL! Displacement in direction " << d << " should be " << P1_displacement[d] << " but is in fact " << P[1].total_displacement()[d] << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    QDPIO::cout << "PASS!  The displacement is 1 3 10 -2!" << std::endl;
    
    
    
    
    QDPIO::cout << banner("P[2]") << std::endl;
    QDPIO::cout << "P[2] is built from an array corresponding to x y z^3 t^-5 x^-2 y x y^-2 t^5 z^-3" << std::endl;
    QDPIO::cout << "It has zero displacement, but is NOT the zero path." << std::endl;
    if ( P[2].is_zero() ){
        QDPIO::cout << "FAIL! That isn't reflected by P[2].is_zero()!" << std::endl;
        QDP_abort(EXIT_FAILURE);
    } else{
        QDPIO::cout << "PASS! That is confirmed by P[2].is_zero()!"    << std::endl;
    }
    for_direction(d) if( 0 != P[2].total_displacement()[d] ) {
        QDPIO::cout << "FAIL! Displacement in direction " << d << " is " << P[2].total_displacement()[d] << "." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    QDPIO::cout << "PASS!  The displacement is the zero vector!" << std::endl;
    
    
    
    
    QDPIO::cout << banner("P[3]") << std::endl;
    QDPIO::cout << "P[3] is built from the string '" << test_path_3 << "'" << std::endl;
    multi1d<int> P3_displacement(Nd);
    P3_displacement[0] = 3; P3_displacement[1] = 2; P3_displacement[2] = -1; P3_displacement[3] = 0;
    QDPIO::cout << "It has displacement 3 0 1 0" << std::endl;
    if ( P[3].is_zero() ){
        QDPIO::cout << "FAIL! That isn't reflected by P[3].is_zero()" << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    for_direction(d) if ( P3_displacement[d] != P[3].total_displacement()[d] ){
        QDPIO::cout << "FAIL! Displacement in direction " << d << " should be " << P3_displacement[d] << " but is in fact " << P[3].total_displacement()[d] << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    QDPIO::cout << "PASS! Displacement checks out." << std::endl;
    std::string P3_short="0:3;1:2;2:-1;";
    std::string P3_long="0:1;0:1;0:1;1:1;1:1;2:-1;";
    QDPIO::cout << "Manually, I find the short string to be " << P3_short << std::endl;
    pass= (P3_short == P[3].short_string());
    QDPIO::cout << ( pass ? "PASS!" : "FAIL!" ) << "                                   " << P[3].short_string() << std::endl;
    if ( ! pass ) { QDP_abort(EXIT_FAILURE); }
    QDPIO::cout << "and the long string to be " << P3_long << std::endl;
    pass= (P3_long  == P[3].long_string() );
    QDPIO::cout << ( pass ? "PASS!" : "FAIL!" ) << "                     " << P[3].long_string()  << std::endl;
    if ( ! pass ) { QDP_abort(EXIT_FAILURE); }
    
    
    
    
    QDPIO::cout << banner("P[4]") << std::endl;
    QDPIO::cout << "P[4] is built from the string '" << test_path_4 << "'" << std::endl;
    QDPIO::cout << "It is the zero path, but contains multiple forks that all need backtracking." << std::endl;
    if ( P[4].is_nonzero() ){
        QDPIO::cout << "FAIL! That isn't reflected in P[4].is_nonzero()!" << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    for_direction(d) if( 0 != P[4].total_displacement()[d] ) {
        QDPIO::cout << "FAIL! Displacement in direction " << d << " is " << P[4].total_displacement()[d] << "." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
    QDPIO::cout << "PASS!  The displacement is the zero vector!" << std::endl;
    QDPIO::cout << "The path-describing strings are: '" << P[4].short_string() << "' and '" << P[4].long_string() << "'." <<std::endl;
    
    
    
    
    QDPIO::cout << banner("Path reversal") << std::endl;
    std::string reversal_target="2:1;1:-2;0:-3;";
    Path R = P[3].reverse();
    QDPIO::cout << "P[3] is built from the string '" << test_path_3      << "'" << std::endl;
    QDPIO::cout << "The reverse() amounts to      '" << R.short_string() << "'" << std::endl;
    pass = ( R.short_string() == reversal_target);
    QDPIO::cout << ( pass ? "PASS!" : "FAIL!" ) << "                         '"<< reversal_target <<"'" << std::endl;
    if ( ! pass ) { QDP_abort(EXIT_FAILURE); }
    
    
    QDPIO::cout << banner("PASS!") << std::endl;
    
    Chroma::finalize();
    return 0;

}