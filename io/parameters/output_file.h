#ifndef _HADRON_PDFS_OUTPUT_H
#define _HADRON_PDFS_OUTPUT_H

#include "io/xml.h"

struct output{
    std::string directory;
    std::string file;
    bool overwrite;
};

void read(XMLReader &xml_input, const std::string &path, output &o){
    read(xml_input, path+"/directory", o.directory);
    read(xml_input, path+"/file", o.file);
    o.overwrite=false;
    try {
        read(xml_input, path+"/overwrite", o.overwrite);
    } catch( const std::string &error ) {
        QDPIO::cout << "By default, not overwriting." << std::endl;
        o.overwrite = false;
    }
}


#endif /* end of include guard: _HADRON_PDFS_OUTPUT_FILE_H */
