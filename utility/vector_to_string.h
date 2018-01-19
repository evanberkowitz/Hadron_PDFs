#ifndef _HADRON_PDFS_UTILITY_VECTOR_TO_STRING
#define _HADRON_PDFS_UTILITY_VECTOR_TO_STRING

#include <string>
#include "utility/int_to_string.h"

std::string inline vector_to_string(const multi1d<int> &vec){
    
    if( vec.size() > 4 ) return "";

    multi1d<std::string> direction(4);
    direction[0] = "x";
    direction[1] = "y";
    direction[2] = "z";
    direction[3] = "t";

    std::string s="";
    
    for(unsigned int d=0; d<vec.size(); d++){
        s+=direction[d]+int_to_string(vec[d]);
    }
    
    return s;
}

#endif /* end of include guard: _HADRON_PDFS_UTILITY_VECTOR_TO_STRING */
