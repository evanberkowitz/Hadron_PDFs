#include "io/xml.h"

// Lifted from Thorsten.


void read_from_xml(XMLReader& xml, const std::string path, std::string& read_value, std::string default_value){
    QDPIO::cout << "Looking for " << path << " in XML..." << std::flush;
    if (xml.count(path)!= 0){
        try{
            read(xml,path,read_value);
            QDPIO::cout << "found!" << std::endl;
            trim_string(read_value);
            QDPIO::cout << "XML string is " << read_value << std::endl;
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

void read_from_xml(XMLReader& xml, const char* path, std::string& read_value, std::string default_value){
    read_from_xml(xml, std::string(path), read_value, default_value);
    trim_string(read_value);
}

// template<class T>
// int readex_safe(XMLReader& xml, const std::string& path, T& readval){
//     if (xml.count(path)!= 0){
//         try{
//             read(xml,path,readval);
//             return EXIT_SUCCESS;
//         }
//         catch(const std::string& e) {
//             QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
//             QDP_abort(1);
//         }
//     }
//     return EXIT_FAILURE;
// }
//
//
// void readex(XMLReader& xml, const std::string& path, const std::string& firsttag, GroupXML_t& readval){
//     try{
//         readval=readXMLGroup(xml,path,firsttag);
//     }
//     catch(const std::string& e) {
//         QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
//         QDP_abort(1);
//     }
// }
//
// void readex(XMLReader& xml, const std::string& path, const std::string& firsttag, multi1d<GroupXML_t>& readval){
//     try{
//         readval=readXMLArrayGroup(xml,path,firsttag);
//     }
//     catch(const std::string& e) {
//         QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
//         QDP_abort(1);
//     }
// }
//
// template<class T>
// void readex_opt(XMLReader& xml, const std::string& path, T& readval, const T& defval){
//     if (xml.count(path)!= 0){
//         try{
//             read(xml,path,readval);
//         }
//         catch(const std::string& e) {
//             QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
//             QDP_abort(1);
//         }
//     }
//     else{
//         readval=defval;
//     }
// }
//
//
// void readex_opt(XMLReader& xml, const std::string& path, std::string& readval, const std::string& defval){
//     if (xml.count(path)!= 0){
//         try{
//             read(xml,path,readval);
//         }
//         catch(const std::string& e) {
//             QDPIO::cerr << "Caught Exception reading XML: " << e << std::endl;
//             QDP_abort(1);
//         }
//     } else {
//         readval=defval;
//     }
// }
