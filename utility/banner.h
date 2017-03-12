#ifndef _HADRON_PDF_UTILITY_BANNER_H
#define _HADRON_PDF_UTILITY_BANNER_H

#include <string>

std::string banner(const std::string title){
    std::string result="\n";
    result+="##########\n";
    result+="########## "+title+"\n";
    result+="##########\n";
    // std::string result=title;
    return result;
}


#endif /* end of include guard: _HADRON_PDF_UTILITY_BANNER_H */
