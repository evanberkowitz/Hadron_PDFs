#ifndef TRIM_STRING_H
#define TRIM_STRING_H

#include "standard_includes.h"

inline void trim_string(std::string &str){
    const std::string whitespace=" \t\f\v\n\r";
    size_t first=str.find_first_not_of(whitespace);
    size_t last=str.find_last_not_of(whitespace);
    str.erase(0,first);
    str.erase((last-first)+1);
}


#endif /* end of include guard: TRIM_STRING_H */
