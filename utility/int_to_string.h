#ifndef _HADRON_PDFS_UTILITY_INT_TO_STRING
#define _HADRON_PDFS_UTILITY_INT_TO_STRING

#include <string>

std::string inline int_to_string(int x){
    return std::to_string(static_cast<long long>(x));
}

#endif /* end of include guard: _HADRON_PDFS_UTILITY_INT_TO_STRING */
