#include "standard_includes.h"
#include "include/Path.h"
#include "include/WilsonLine.h"

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




    // QDPIO::cout << banner("gauge_shift") << std::endl;
    //
    // QDPIO::cout << "Create a non-interacting gauge field..." << std::flush;
    // Real one=1.0;
    // multi1d<LatticeColorMatrix> links(Nd);
    // for_direction(d){ links[d] = one; }
    // QDPIO::cout << "done!" << std::endl;
    //
    // QDPIO::cout << "Create multi1d<int>s for O the origin and for N the neighbor" << std::endl;
    // multi1d<int> origin(Nd);
    // multi1d<int> neighbor(Nd);
    // for_direction(d){ origin[d] = 0; neighbor[d] = 0; }
    // neighbor[0] = 1;
    // QDPIO::cout << "O: " << std::flush; for_direction(d) QDPIO::cout << origin[d] << " " << std::flush;  QDPIO::cout << std::endl;
    // QDPIO::cout << "N: " << std::flush; for_direction(d) QDPIO::cout << neighbor[d] << " " << std::flush;  QDPIO::cout << std::endl;
    //
    // LatticeColorMatrix U=zero;
    // ColorMatrix color_temp=zero;
    // Real r = 0.723;  // chosen randomly :)
    // QDPIO::cout << "We set U to be " << r << " times the unit matrix at the origin:" << std::endl;
    // for(int c1 = 0; c1 < 3; c1++){
    //     pokeColor(color_temp,r,c1,c1);
    // }
    // pokeSite(U, color_temp, origin);
    // for(int c1 = 0; c1 < 3; c1++){
    //     for(int c2 = 0; c2 < 3; c2++){
    //         QDPIO::cout << peekColor(peekSite(U,origin),c1,c2) << " ";
    //     }
    //     QDPIO::cout << std::endl;
    // }
    //
    //
    // int s   = -1;
    // int dir = 0;
    //
    // QDPIO::cout << "Now let V <-- gauge transport of U with sign " << s << " along direction " << dir << std::endl;
    // LatticeColorMatrix V=gauge_shift(links, U, s, dir);
    // QDPIO::cout << "In V we " << ( s > 0 ? "expect" : "don't expect") << " to see something" << std::endl;
    // for(int c1 = 0; c1 < 3; c1++){
    //     for(int c2 = 0; c2 < 3; c2++){
    //         QDPIO::cout << peekColor(peekSite(V,neighbor),c1,c2) << " ";
    //     }
    //     QDPIO::cout << std::endl;
    // }



    QDPIO::cout << banner("WilsonLine") << std::endl;

    WilsonLine L1(test_path_1);
    QDPIO::cout << "L1 is the same as P1." << std::endl;
    QDPIO::cout << "P1: " << P1.short_string() << std::endl;
    QDPIO::cout << "L1: " << L1.short_string() << std::endl;




    QDPIO::cout << banner("Finalize") << std::endl;
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
    
    swatch_everything.stop();
    QDPIO::cout << "Everything took " << swatch_everything.getTimeInSeconds() << " seconds." << std::endl;
    
    
    return 0;
}