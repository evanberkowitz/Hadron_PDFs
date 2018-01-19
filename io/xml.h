#ifndef _HADRON_PDFS_XML_H
#define _HADRON_PDFS_XML_H

#include "standard_includes.h"

// Lifted from Thorsten and modified.

void read_from_xml(XMLReader& xml, const std::string path, std::string& read_value, std::string defaut_value="");
void read_from_xml(XMLReader& xml, const char* path, std::string& read_value, std::string defaut_value="");

template<class T>
void read_from_xml(XMLReader& xml, const std::string& path, T& read_value){
    QDPIO::cout << "Looking for " << path << " in XML..." << std::flush;
    if (xml.count(path)!= 0){
        try{
            read(xml,path,read_value);
            QDPIO::cout << "found!" << std::endl;
        }
        catch(const std::string& e) {
            QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
            QDP_abort(1);
        }
    } else {
        QDPIO::cout << "not found.  No default value." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }
}

template<class T>
void read_with_default(XMLReader& xml, const std::string& path, T& read_value, const T& default_value){
    QDPIO::cout << "Looking for " << path << " in XML..." << std::flush;
    if (xml.count(path)!= 0){
        try{
            read(xml,path,read_value);
            QDPIO::cout << "found!" << std::endl;
        }
        catch(const std::string& e) {
            QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
            QDP_abort(1);
        }
    } else {
        QDPIO::cout << "not found.  Using default value." << std::endl;
        read_value=default_value;
    }
}


#endif /* end of include guard: _HADRON_PDFS_XML_H */
