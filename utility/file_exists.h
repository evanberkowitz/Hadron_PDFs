#ifndef FILE_EXISTS_H
#define FILE_EXISTS_H

#include "standard_includes.h"

// inline bool file_exists(const std::string& name){
//     if (FILE *file = fopen(name.c_str(), "r")){
//         fclose(file);
//         return true;
//     } else {
//         return false;
//     }
// }

inline bool file_exists(const std::string& name)
{
//  Should be made mpi-friendly.
//    unsigned int mynode=Layout::nodeNumber();
//    if( 0 == mynode)
    std::ifstream infile(name.c_str());
    return infile.good();
}


#endif /* end of include guard: FILE_EXISTS_H */
