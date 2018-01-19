#ifndef _LATTICE_PARAMETERS_H
#define _LATTICE_PARAMETERS_H

#include "io/xml.h"
#include "standard_includes.h"

struct lattice_parameters {
    multi1d<int>    dimensions;
    int             time_direction;
    int             time_slices;
};

QDP::StandardOutputStream& operator << (QDP::StandardOutputStream &o, const lattice_parameters &lattice){
    o << "The lattice has dimensions ";
    for(int i = 0; i< lattice.dimensions.size(); i++) {
        o << lattice.dimensions[i] << " ";
    };
    o << "\nTime is the " << lattice.time_direction << " direction, of length " << lattice.time_slices;
    return o;
}


void read(XMLReader& xml_input, const std::string &path, lattice_parameters &target){
    
    QDPIO::cout << "\nReading in lattice parameters..." << std::endl;
    
    read(xml_input, path+"/dimensions", target.dimensions);
    // read_with_default(xml_input, path+"/time_direction", target.time_direction, target.dimensions.size()-1);
    target.time_direction = target.dimensions.size()-1;
    target.time_slices = target.dimensions[target.time_direction];
    
    QDPIO::cout << "Lattice parameters read!\n" << std::endl;
    QDPIO::cout << target << std::endl;
}

#endif /* end of include guard: _LATTICE_PARAMETERS_H */