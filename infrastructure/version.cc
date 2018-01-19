#include "infrastructure/version.h"

std::string Hadron_PDFs_compilation(){
    std::string info="Hadron_PDFs COMPILATION INFORMATION\n";
    info+="========================================================================================\n";
    info+="git branch:          "+std::string(GIT_BRANCH)+"\n";
    info+="git commit:          "+std::string(GIT_LATEST_COMMIT)+"\n";
    info+="git commit time:     "+std::string(GIT_COMMIT_TIME)+"\n";
    info+="compilation time:    "+std::string(__DATE__)+" "+std::string(__TIME__)+"\n";
    if( "0" != std::string(GIT_PORCELAIN)){
    info+="WARNING:             COMPILED AGAINST AN UNCLEAN REPO\n";
    info+="dirty files:         "+std::string(GIT_PORCELAIN)+"\n";
    }
    info+="========================================================================================\n";

    return info;
}
