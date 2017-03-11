#ifndef _VERSION_H
#define _VERSION_H

#define my_str(s) #s

#ifndef GIT_BRANCH
#define GIT_BRANCH "unspecified"
#endif

#ifndef GIT_LATEST_COMMIT
#define GIT_LATEST_COMMIT "unspecified"
#endif

#ifndef GIT_COMMIT_TIME
#define GIT_COMMIT_TIME "unspecified"
#endif

std::string Hadron_PDFs_compilation(){
    std::string info="HADRON_PDFs INFO\n";
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

#endif // _VERSION_H