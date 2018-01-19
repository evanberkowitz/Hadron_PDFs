#ifndef _HADRON_PDFS_POINT_SOURCE
#define _HADRON_PDFS_POINT_SOURCE

LatticePropagator PointSource(const multi1d<int>& pos, const int& time_dir){
    START_CODE();

    QDPIO::cout << "Constructing point source at " << std::flush;
    for(unsigned int i=0; i<pos.size(); i++) QDPIO::cout << pos[i] << " " << std::flush;
    QDPIO::cout << "..." << std::flush;
    //result vector:
    LatticePropagator result=zero;  
    for(unsigned int c=0; c<Nc; c++){
        //fill lattice color vector with a one at pos:
        LatticeColorVector tmpcolorvec=zero;
        srcfil(tmpcolorvec,pos,c);

        //inject in larger source:
        for(unsigned int s=0; s<Ns; s++){
            LatticeFermion tmpferm=zero;
            CvToFerm(tmpcolorvec,tmpferm,s);
            FermToProp(tmpferm,result,c,s);
        }
    }
    
    QDPIO::cout << "done!" << std::endl;
    
    END_CODE();

    return result;
}




#endif /* end of include guard: _HADRON_PDFS_POINT_SOURCE */
