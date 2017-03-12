#include "utility/banner.h"

std::string banner(const std::string title){
    std::string result="\n";
    result+="##########\n";
    result+="########## "+title+"\n";
    result+="##########\n";
    // std::string result=title;
    return result;
}