#include "Hadron_PDFs.h"
#include "standard_includes.h"
#include "include/PointSource.h"
#include "io/parameters/wilson_line.h"
#include "include/ParallelTransporter.h"
#include "include/Photon.h"


// #include "include/SpinBasis.h"
// #include "include/Action.h"
// #include "include/Solver.h"

#define FERMION  LatticeFermion, multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix>
#define STRIPE   8388608 // 8 MB


int main(int argc, char **argv){
    
    StopWatch swatch_everything;
    swatch_everything.reset();
    swatch_everything.start();
    
    Hadron_PDFs::initialize(&argc, &argv);
    XMLReader XML;
    XML.open(Chroma::getXMLInputFileName());
    
    lattice_parameters lattice;
    read(XML, "/Hadron_PDFs/lattice", lattice);
    
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
    std::string source="point("+vector_to_string(source_position)+")";
    QDPIO::cout << "Source string = " << source << std::endl;
    
    LatticePropagator point_source = zero;
    point_source = PointSource(source_position, Nd-1);
    
    QDPIO::cout << "Creating shifter to zero correlators..." << std::flush;
    // If the source is on t=8, we want to use the parallel transporter to move data from t=8 to t=0.
    // Because of the whole QDP sense of shifts, that's a POSITIVE shift:
    std::string time_shift=int_to_string(lattice.time_direction)+":+"+int_to_string(source_position[lattice.time_direction])+";";
    QDPIO::cout << " using " << time_shift << "... " << std::flush;
    ParallelTransporter shift_source_to_time_zero(U, time_shift);
    QDPIO::cout << "done!" << std::endl;
    
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
    
    multi1d<wilson_line_operator> operators;
    read(XML, "/Hadron_PDFs/operators", operators);
    
    
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
#ifdef DEBUG
    H5.mkdir("/transporters");
#endif
    
    std::string output_path="/";
    
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
                push(xml_out, "all_from_"+source);
                timed("initial solve") S.quarkProp(all_from_point, xml_out, point_source, state, Solver_Parameters, quarkSpinType, ncg_had);
                xml_out.flush();
                pop(xml_out);
                
                H5.write("/propagators/all_from_"+source, all_from_point, HDF5Base::trunc);
                
        QDPIO::cout << "#         Build the pion correlator, as usual." << std::endl;
                LatticeComplex pion = trace( all_from_point * adj(all_from_point) );
                LatticeComplex pion_shifted = shift_source_to_time_zero(pion);
        // QDPIO::cout << "#         Build the nucleon correlator, as usual." << std::endl;
        //         LatticeSpinMatrix nucleon = zero;
        //         // nucleon += ...
        //         H5.write("/nucleon",nucleon, HDF5Base::trunc);
        QDPIO::cout << "#         Write out the correlators and the propagator (only keep the current propagator)." << std::endl;
                H5.write("/pions/piPlus(all)_piPlus("+source+")", pion_shifted, HDF5Base::trunc);
                
        QDPIO::cout << "#         For each operator (photon, wilson_line, photon)" << std::endl;
                for(unsigned int o=0; o < operators.size(); o++)
                {
                    LatticePropagator temp=zero;
                    LatticePropagator fh_source=zero;
                    
                    std::string description="";
                    std::string photon_in ="photon("+vector_to_string(operators[o].first_photon)+")";
                    std::string photon_out="photon("+vector_to_string(operators[o].second_photon)+")";
                    std::string wilson = "wilson("+operators[o].path+")";
                    
        QDPIO::cout << "#             Apply the incoming photon vertex to the propagator." << std::endl;
                    QDPIO::cout << "Creating first photon..." << std::flush;
                    LatticeComplex first_photon = phases(operators[o].first_photon);
                    QDPIO::cout << " momentum is " << vector_to_string(operators[o].first_photon) << std::endl;
                    QDPIO::cout << "Injecting momentum..." << std::flush;
                    fh_source = first_photon * all_from_point;
                    QDPIO::cout << "done!" << std::endl;
                    
        QDPIO::cout << "#             Parallel transport the propagator along the path." << std::endl;
                    QDPIO::cout << "Creating parallel transporter for path " << operators[o].path << std::endl;
                    ParallelTransporter transport(U, operators[o].path);
                    multi1d<int> displacement = transport.total_displacement();
                    QDPIO::cout << "Path has total displacement" << std::flush;
                    for(unsigned int d=0; d<displacement.size(); d++) QDPIO::cout << " " << displacement[d] << std::flush;
                    QDPIO::cout << std::endl;
#ifdef DEBUG
                    QDPIO::cout << "Writing transporter..." << std::flush;
                    H5.mkdir("/transporters/"+wilson);
                    transport.write(H5, "/transporters/"+wilson);
                    QDPIO::cout << "done!" << std::endl;
#endif
                    QDPIO::cout << "Gauge transporting quark..." << std::flush;
                    temp = Gamma(operators[o].gamma) * transport(fh_source);
                    QDPIO::cout << "done!" << std::endl;
                    
        QDPIO::cout << "#             Apply the outgoing photon vertex to the propagator." << std::endl;
                    QDPIO::cout << "Creating second photon..." << std::flush;
                    LatticeComplex second_photon = phases(operators[o].second_photon);
                    QDPIO::cout << " momentum is " << vector_to_string(operators[o].second_photon) << std::endl;
                    QDPIO::cout << "Injecting momentum..." << std::flush;
                    fh_source = second_photon*temp;
                    QDPIO::cout << "done!" << std::endl;
                    
#ifdef DEBUG
                    H5.write("/propagators/FH_SOURCE_gamma"+int_to_string(operators[o].gamma)+"_"+photon_out+"_"+wilson+"_"+photon_in+"_all_from_"+source, fh_source, HDF5Base::trunc );
#endif
                    
                    QDPIO::cout << "Allocating new propagator..." << std::flush;
                    LatticePropagator all_from_W_all_from_point = zero;
                    QDPIO::cout << "done!" << std::endl;
                    
                    QDPIO::cout << "#             Re-solve the Dirac equation using that propagator as the source." << std::endl;
                    
                    // SOL(all_from_W_all_from_point, W_all_from_point);
                    std::string fh_prop="all_from_gamma"+int_to_string(operators[o].gamma)+photon_out+"_"+wilson+"_"+photon_in+"_all_from_"+source;
                    push(xml_out, fh_prop);
                    timed("continued solve "+operators[o].path) S.quarkProp(all_from_W_all_from_point, xml_out, fh_source, state, Solver_Parameters, quarkSpinType, ncg_had);
                    xml_out.flush();
                    pop(xml_out);
                    
                    H5.write("/propagators/"+fh_prop, all_from_W_all_from_point, HDF5Base::trunc);
                    
        QDPIO::cout << "#             Use that propagator (and the original propagator) to build the pion and nucleon correlators." << std::endl;                    
                    
                    LatticeComplex pion_A = - trace( all_from_W_all_from_point * adj(all_from_point) );
                    
        QDPIO::cout << "#             This yields full-volume correlators, " << std::endl;
        QDPIO::cout << "#                  <   destroyed at every spacetime location" << std::endl;
        QDPIO::cout << "#                      hard outgoing photon with definite momentum" << std::endl;
        QDPIO::cout << "#                      gamma * Wilson line" << std::endl;
        QDPIO::cout << "#                      hard ingoing photon with definite momentum" << std::endl;
        QDPIO::cout << "#                      created at point >" << std::endl;
        QDPIO::cout << "#             For each sink-side momentum, we know the source-side momentum by conservation." << std::endl;
        QDPIO::cout << "#             Write out the full volume correlator." << std::endl;
                    
                    LatticeComplex pion_A_shifted = shift_source_to_time_zero(pion_A);
                    H5.write("/pions/piPlus(all)_"+photon_out+"_dbar_"+wilson+"_d_"+photon_in+"_piPlus("+source+")", pion_A_shifted, HDF5Base::trunc);
                    
        QDPIO::cout << "#             Do analysis, including fourier transform to momentum space, offline." << std::endl;
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