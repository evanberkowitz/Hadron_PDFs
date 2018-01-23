#include "Hadron_PDFs.h"
#include "standard_includes.h"
#include "include/ParallelTransporter.h"
#include "include/Photon.h"

// #include "include/SpinBasis.h"
#include "include/Action.h"
#include "include/Solver.h"

#define FERMION  LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix>
#define STRIPE   8388608 // 8 MB

struct photon_pair{
    multi1d<int> incoming;
    multi1d<int> outgoing;
};

void read(XMLReader &xml_input, const std::string &path, photon_pair &p){
    QDPIO::cout << "\nReading in photon pair..." << std::endl;
    read(xml_input, path+"/incoming",  p.incoming);
    read(xml_input, path+"/outgoing", p.outgoing);
    QDPIO::cout << "Incoming momentum " << std::flush;
    for(unsigned int i=0; i < p.incoming.size() ; i++) QDPIO::cout << p.incoming[i] << " " << std::flush;
    QDPIO::cout << std::endl;
    QDPIO::cout << "Outgoing momentum " << std::flush;
    for(unsigned int i=0; i < p.outgoing.size() ; i++) QDPIO::cout << p.outgoing[i] << " " << std::flush;
    QDPIO::cout << std::endl;
}

int main(int argc, char **argv){
    
    StopWatch swatch_everything;
    swatch_everything.reset();
    swatch_everything.start();
    
    Hadron_PDFs::initialize(&argc, &argv);
    XMLReader XML;
    XML.open(Chroma::getXMLInputFileName());
    
    QDPIO::cout << banner("Get the configuration:") << std::endl;
    
    multi1d<LatticeColorMatrix> U(Nd);
    Cfg_t configuration;
    read(XML, "/Hadron_PDFs/configuration", configuration);
    bool randomize_configuration=false;
    try {
        read(XML, "/Hadron_PDFs/configuration/randomize", randomize_configuration);
    } catch( const std::string &error ) {
        QDPIO::cout << "Picking the default for random gauge transformations: false" << std::endl;
        randomize_configuration = false;
    }
    
    
    QDPIO::cout << "Configuration:" << configuration.cfg_file << std::endl;
    QDPIO::cout << "    reading..." << std::endl;
    XMLReader gauge_file_xml, gauge_xml;
    gaugeStartup(gauge_file_xml, gauge_xml, U, configuration);
    unitarityCheck(U);
    
    QDPIO::cout << configuration.cfg_file << " read successfully, unitarity check passed." << std::endl;
    
    if(randomize_configuration){
        QDPIO::cout << "Doing a random gauge transformation... " << std::flush;
        rgauge(U);
        QDPIO::cout << "done!" << std::endl;
    } else {
        QDPIO::cout << "Not applying any random gauge transformation. " << std::endl;
    }
    
    
    
    QDPIO::cout << banner("Construct the source:") << std::endl;
    
    multi1d<int> source_position;
    read(XML, "/Hadron_PDFs/source/position", source_position);
    if( Nd != source_position.size() ){
        QDPIO::cout << "Source position underspecified: " << std::flush;
        for(unsigned int i=0; i<source_position.size(); i++) QDPIO::cout << source_position[i] << " " << std::flush;
        QDPIO::cout << std::endl;
        QDPIO::cout << "                                " <<Nd << " coordinates required." << std::endl;
        QDP_abort(-1);
    }
    QDPIO::cout << "Source position = " << std::flush;
    for(unsigned int i=0; i<source_position.size(); i++) QDPIO::cout << source_position[i] << " " << std::flush;
    QDPIO::cout << std::endl;
    
    LatticePropagator point_source = zero;
    point_source = PointSource(source_position, Nd-1);
    
    
    
    QDPIO::cout << banner("Construct action, solver:") << std::endl;
    
    SimpleFermBCParams fermion_boundary_conditions;
    read(XML, "/Hadron_PDFs/lattice/fermionic_boundary_conditions", fermion_boundary_conditions.boundary);
    QDPIO::cout << "Fermionic boundary conditions: " << std::flush;
    for (unsigned int i=0; i<Nd; i++)    QDPIO::cout << fermion_boundary_conditions.boundary[i] << " " << std::flush;
    QDPIO::cout << std::endl;
    Handle< FermBC< FERMION > > Light_Boundary_Condition_Handle(new SimpleFermBC< FERMION >(fermion_boundary_conditions));
    Handle<CreateFermState< FERMION >> Light_Fermion_State(new CreateSimpleFermState< FERMION >(Light_Boundary_Condition_Handle));
    
    Real mass=0.;
    read(XML, "/Hadron_PDFs/FermionAction/light/Mass", mass);
    QDPIO::cout << "Light quark mass = " << mass << std::endl;
    
    QDPIO::cout << "Constructing action..." << std::flush;
    EvenOddPrecWilsonFermAct S(Light_Fermion_State, mass);
    QDPIO::cout << "done!" << std::endl;
    
    QDPIO::cout << "Reading high precision inverter..." << std::flush;
    XMLReader read_solver(XML, "/Hadron_PDFs/Inverter");
    GroupXML_t Solver_Parameters = readXMLGroup(read_solver, "HighPrecision", "invType");
    QDPIO::cout << "done!" << std::endl;
    
    // Action A(XML, "/Hadron_PDFs/FermionAction/light");
    // Solver SOL(A, Solver_Parameters, U);
    
    QDPIO::cout << "Constructing state..." << std::flush;
    Handle<FermState< FERMION > > state(S.createState(U));
    QDPIO::cout << "done!" << std::endl;
    
    QuarkSpinType quarkSpinType = QUARK_SPIN_TYPE_FULL;
    int ncg_had = 0;
    
    
    
    QDPIO::cout << banner("Determine observables...") << std::endl;
    
    multi1d<std::string> path_specifier;
    read(XML, "/Hadron_PDFs/paths", path_specifier);
    for(unsigned int p=0; p < path_specifier.size(); p++) QDPIO::cout << "Path " << p << " " << path_specifier[p] << std::endl;
    
    multi1d<photon_pair> photons;
    read(XML, "/Hadron_PDFs/photons", photons);
    
    
    
    QDPIO::cout << banner("Prepare output file...") << std::endl;
    
    std::string output_directory="";
    std::string output_file="";
    read(XML, "/Hadron_PDFs/output/directory", output_directory);
    read(XML, "/Hadron_PDFs/output/file", output_file);
    bool overwrite=false;
    try {
        read(XML, "/Hadron_PDFs/output/overwrite", overwrite);
    } catch( const std::string &error ) {
        QDPIO::cout << "By default, not overwriting." << std::endl;
        overwrite = false;
    }
    
    QDPIO::cout << "Output directory is " << output_directory << std::endl;
    QDPIO::cout << "Output file is      " << output_file << std::endl;
    if( file_exists(output_directory+"/"+output_file) && ! overwrite){
        QDPIO::cout << "File exists and overwrite isn't true." << std::endl;
        QDPIO::cout << "Exiting for safety" << std::endl;
        QDP_abort(-1);
    }
    HDF5Base::writemode io_mode=HDF5Base::ate;
    if( overwrite && file_exists(output_directory+"/"+output_file) ) {
        QDPIO::cout << "Overwriting!" << std::endl;
        io_mode=HDF5Base::trunc;
    }
    
    HDF5Writer H5;
    H5.open(output_directory+"/"+output_file, io_mode);
    H5.set_stripesize(STRIPE);
    H5.mkdir("/propagators");
    H5.mkdir("/pions");
    H5.mkdir("/transporters");
    
    QDPIO::cout << banner("Main Loop:") << std::endl;
    
    XML.close();
    read_solver.close();
    
    timed("Hadron_PDFs"){
        
        QDPIO::cout << "# Currently I describe the isospin limit, and consider light quarks only." << std::endl;
        
        QDPIO::cout << "# For the configuration" << std::endl;
        timed("Configuration"){
        QDPIO::cout << "#     For each source position" << std::endl;        
            timed("Source position"){
        QDPIO::cout << "#         Get a source-to-all propagator S." << std::endl;
                LatticePropagator all_from_point = zero;
                XMLFileWriter xml_out(output_directory+"/propagators.out.xml"); //TODO: make source-dependent?
                push(xml_out, "propagators");
                push(xml_out, "all_from_point");
                timed("initial solve") S.quarkProp(all_from_point, xml_out, point_source, state, Solver_Parameters, quarkSpinType, ncg_had);
                xml_out.flush();
                pop(xml_out);
                // SOL(all_from_point, point_source);
                
                H5.write("/propagators/all_point", all_from_point, HDF5Base::trunc);
                
        QDPIO::cout << "#         Build the pion correlator, as usual." << std::endl;
                LatticeComplex pion = trace( all_from_point * adj(all_from_point) );
                H5.write("/pions/two_point", pion, HDF5Base::trunc);
        // QDPIO::cout << "#         Build the nucleon correlator, as usual." << std::endl;
        //         LatticeSpinMatrix nucleon = zero;
        //         // nucleon += ...
        //         H5.write("/nucleon",nucleon, HDF5Base::trunc);
        QDPIO::cout << "#         Write out the correlators and the propagator (only keep the current propagator)." << std::endl;
        
        QDPIO::cout << "#         Do in a checkpointed way:" << std::endl;
        QDPIO::cout << "#         For each incoming photon (momentum and spinor index)" << std::endl;
        QDPIO::cout << "#         For each path you want to parallel transport your quark along" << std::endl;
        QDPIO::cout << "#         For each outgoing photon (momentum and spinor index)" << std::endl;
                for(unsigned int q=0; q < photons.size(); q++)
                for(unsigned int p=0; p < path_specifier.size(); p++)
                {
                    std::string description = "all_photon("+vector_to_string(photons[q].outgoing)+")_wilson("+path_specifier[p]+")_photon("+vector_to_string(photons[q].incoming)+")_pt";
                    
                    QDPIO::cout << "Creating first photon..." << std::flush;
                    // LatticeComplex first_photon = phases(photons[q].incoming);
                    QDPIO::cout << " momentum is " << vector_to_string(photons[q].incoming) << std::endl;
                    
                    QDPIO::cout << "Creating parallel transporter for path " << path_specifier[p] << std::endl;
                    ParallelTransporter transport(U, path_specifier[p]);
                    multi1d<int> displacement = transport.total_displacement();
                    QDPIO::cout << "Path has total displacement" << std::flush;
                    for(unsigned int d=0; d<displacement.size(); d++) QDPIO::cout << " " << displacement[d] << std::flush;
                    QDPIO::cout << std::endl;
                    QDPIO::cout << "Writing transporter..." << std::flush;
                    H5.mkdir("/transporters/wilson("+path_specifier[p]+")");
                    transport.write(H5, "/transporters/wilson("+path_specifier[p]+")");
                    QDPIO::cout << "done!" << std::endl;
                    
                    QDPIO::cout << "Creating second photon..." << std::flush;
                    // LatticeComplex second_photon = phases(photons[q].outgoing);
                    QDPIO::cout << " momentum is " << vector_to_string(photons[q].outgoing) << std::endl;
                    
        QDPIO::cout << "#             Apply the incoming photon vertex to the propagator." << std::endl;
        QDPIO::cout << "#             Parallel transport the propagator along the path." << std::endl;
                    
                    QDPIO::cout << "Transporting quark..." << std::flush;
                    LatticePropagator W_all_from_point = Gamma(8)*transport(all_from_point);
                    QDPIO::cout << "done!" << std::endl;
                    H5.write("/propagators/FH_SOURCE_"+path_specifier[p], W_all_from_point, HDF5Base::trunc );
                    
                    QDPIO::cout << "Allocating new propagator..." << std::flush;
                    LatticePropagator all_from_W_all_from_point = zero;
                    QDPIO::cout << "done!" << std::endl;
                    
                    QDPIO::cout << "#             Apply the outgoing photon vertex to the propagator." << std::endl;
                    QDPIO::cout << "#             Re-solve the Dirac equation using that propagator as the source." << std::endl;
                    
                    // SOL(all_from_W_all_from_point, W_all_from_point);
                    push(xml_out, "fh_prop_"+path_specifier[p]);
                    timed("continued solve "+path_specifier[p]) S.quarkProp(all_from_W_all_from_point, xml_out, W_all_from_point, state, Solver_Parameters, quarkSpinType, ncg_had);
                    xml_out.flush();
                    pop(xml_out);
                    
                    H5.write("/propagators/"+description, W_all_from_point, HDF5Base::trunc);
                    
        QDPIO::cout << "#             Use that propagator (and the original propagator) to build the pion and nucleon correlators.  Fourier transform." << std::endl;                    
                    
                    LatticeComplex pion_A = - trace( all_from_W_all_from_point * adj(all_from_point) );
                    
        QDPIO::cout << "#             This yields full-volume correlators, " << std::endl;
        QDPIO::cout << "#                  <   destroyed with every momentum (p)" << std::endl;
        QDPIO::cout << "#                      hard outgoing photon with definite momentum" << std::endl;
        QDPIO::cout << "#                      Wilson line" << std::endl;
        QDPIO::cout << "#                      hard ingoing photon with definite momentum" << std::endl;
        QDPIO::cout << "#                      created at point >" << std::endl;
        QDPIO::cout << "#             For each sink-side momentum, we know the source-side momentum by conservation." << std::endl;
        QDPIO::cout << "#             Write out the full (momentum-)volume correlator." << std::endl;
                    
                    H5.write("/pions/A_"+description, pion_A, HDF5Base::trunc);
                    
        QDPIO::cout << "#             Do analysis offline." << std::endl;
                }
                
                pop(xml_out);
                xml_out.close();
            }
        }
    }
    
    
    
    
    QDPIO::cout << banner("Finalize") << std::endl;
    H5.close();
    Chroma::finalize();
    QDPIO::cout << "done!" << std::endl;
    
    swatch_everything.stop();
    QDPIO::cout << "Everything took " << swatch_everything.getTimeInSeconds() << " seconds." << std::endl;
    
    return 0;
}