#ifndef _HADRON_PDFS_WILSON_LINE
#define _HADRON_PDFS_WILSON_LINE

struct wilson_line_operator{
    multi1d<int> first_photon;
    std::string  path;
    int gamma;
    multi1d<int> second_photon;
};

void read(XMLReader &xml_input, const std::string &path, wilson_line_operator &o){
    QDPIO::cout << "\nReading in wilson line operator..." << std::endl;
    read(xml_input, path+"/first_photon",   o.first_photon);
    read(xml_input, path+"/path",           o.path);
    read(xml_input, path+"/gamma",          o.gamma);
    read(xml_input, path+"/second_photon",  o.second_photon);
    
    QDPIO::cout << "First photon:  " << std::flush;
    for(unsigned int i=0; i < o.first_photon.size() ; i++) QDPIO::cout << o.first_photon[i] << " " << std::flush;
    QDPIO::cout << std::endl;
    QDPIO::cout << "Path:          " << o.path << std::endl;
    QDPIO::cout << "Gamma matrix:  " << o.gamma << std::endl;
    QDPIO::cout << "Second photon: " << std::flush;
    for(unsigned int i=0; i < o.second_photon.size() ; i++) QDPIO::cout << o.second_photon[i] << " " << std::flush;
    QDPIO::cout << std::endl;
}


#endif /* end of include guard: _HADRON_PDFS_WILSON_LINE */
