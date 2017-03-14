#include "standard_includes.h"
#include "include/Path.h"

std::string multi2d_int_to_string(const multi2d<int> &P){
    std::string result="";
    if( P.nrows() == 0) return "ZERO";
    for(int r=0; r<P.nrows(); r++){
        for_direction(d){
            if( P[r][d] != 0) result+=int_to_string(d)+": "+int_to_string(P[r][d])+"; ";
        }
    }
    return result;
}

multi2d<int> path_compress(const multi2d<int> &P){
    // Create a temporary array.
    multi2d<int> temp(P.nrows(), Nd);

    // Zero it out
    for( int r=0; r < temp.nrows(); r++){
        for_direction(d){ temp[r][d]=0; }
    }

    int zipped_length=0;
    int streak_end=0;
    for( int r=0; r < P.nrows(); r=streak_end){
        int current_direction=0;
        int total_steps=0;
        
        // Find the direction this step is in
        for_direction(d){ if( P[r][d] !=0 ) current_direction=d; }
        total_steps+=P[r][current_direction];
        // Look at all the next steps...
        for( streak_end=r+1; streak_end < P.nrows(); streak_end++){
            // until you find one that's in a different direction...
            if( P[streak_end][current_direction] == 0) break; // and fast-forward the for loop.
            // Those in the same direction can simply be added together.
            total_steps+=P[streak_end][current_direction];
        }
        
        // If the steps totally undo one another, we can forget about them completely.
        // Otherwise, should write down the result and remember that we need a row in the final result.
        if( total_steps !=0 ){
            temp[zipped_length][current_direction]=total_steps;
            zipped_length+=1;
        }
    }
    
    // If P doesn't get smaller upon compression, we're finished:
    if( P.nrows() == zipped_length ){ return P; }

    // Otherwise, we need to copy temp into a correctly-sized array:
    multi2d<int> result(zipped_length, Nd);
    for( int r=0; r < zipped_length; r++){
        for_direction(d){ result[r][d] = temp[r][d]; }
    }
    // Note that backtracking might unlock additional compression:
    return path_compress(result);
}

multi2d<int> path_expand(const multi2d<int> &P){
    int unit_steps=0;
    // Discover how many unit steps we need:
    for(int r=0; r < P.nrows(); r++){
        for_direction(d) unit_steps+=abs(P[r][d]);
    }
    
    // Create and zero-out an array to store the answer.
    multi2d<int> result(unit_steps,Nd);
    for(int r=0; r < unit_steps; r++){
        for_direction(d) result[r][d]=0;
    }
    
    
    int row=0;
    // For each step of arbitrary length
    for(int r=0; r < P.nrows(); r++){
        for_direction(d){
            // unwind it into unit steps
            for(int step=0; step < abs(P[r][d]); step++){
                result[row][d] = sign(P[r][d]);
                // and store each unit step in its own row.
                row++;
            }
        }
    }
    
    return result;
}

multi1d<int> disp(const multi2d<int> &P){
    multi1d<int> result(Nd);
    for_direction(d) result[d]=0;
    for(int r=0; r<P.nrows(); r++){
        for_direction(d) result[d]+=P[r][d];
    }
    return result;
}

Path::Path(){
    
    zero_path=true;
    
    displacement.resize(Nd);
    dx.resize(0,Nd);
    compressed.resize(0,Nd);
    
    for_direction(dir){
        displacement[dir]=0;
    }
    
}

Path::Path(const Path &p){
    zero_path=p.zero_path;

    for_direction(d) displacement[d]=p.displacement[d];

    dx.resize(p.dx.nrows(),p.dx.ncols());
    for(int r = 0; r < dx.nrows(); r++){
        for_direction(d) dx[r][d] = p.dx[r][d];
    }

    compressed.resize(p.compressed.nrows(),p.compressed.ncols());
    for(int r = 0; r < compressed.nrows(); r++){
        for_direction(d) compressed[r][d] = p.compressed[r][d];
    }

};

Path::Path(const multi2d<int> &P){
    START_CODE();
    
    displacement.resize(Nd);
    for_direction(dir) displacement[dir]=0;

    // Check that P is Nd columns wide, so that each step specifies a complete direction.
    // If P passes this test, rather than comparing to P.ncols() we can compare to Nd.
    if( P.ncols() != Nd){
        QDPIO::cout << "Paths must specify "<< Nd <<"-dimensional vectors (the dimension of spacetime)." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }

    // Check that every step is along the lattice directions (or the zero vector):
    for( int r=0; r<P.nrows(); r++){
        int nonzeros=0;
        for_direction(c){ if( P[r][c] != 0) nonzeros++; }
        if( nonzeros > 1){
            // Complain when there is more than one nonzero:
            QDPIO::cout << "Paths must be along the unit vectors of the lattice." << std::endl;
            QDPIO::cout << "[ ";
            for( int c=0; c< Nd; c++) QDPIO::cout << P[r][c] << " ";
            QDPIO::cout << "] is not a valid step." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
    }

    compressed=path_compress(P);
    displacement=disp(compressed);

    if( compressed.nrows() == 0){
        zero_path=true;
        dx.resize(0,Nd);
    }
    else{
        zero_path=false;
        dx=path_expand(compressed);
    }
    END_CODE();
}

Path::Path(const std::string  &P){
    START_CODE();
    
    // Delete all whitespace.
    std::string no_whitespace="";
    for( int i = 0; i < P.length(); i++){
        std::string c=P.substr(i,1);
        if( " " != c && "\t" != c && "\n" != c) no_whitespace+=c;
    }

    // Count how many semicolon-terminated tokens we have:
    int tokens=0;
    for( int i = 0; i < no_whitespace.length(); i++){
        if( ';' == no_whitespace[i] ) tokens++;
    }
    // The user might forget the final semicolon.  Check and repair:
    if( ";" != no_whitespace.substr(no_whitespace.size()-1,1)){ tokens++; no_whitespace+=";"; }

    // Zero out a temporary array to store parsed results:
    multi2d<int> temp(tokens, Nd);
    for(int r =0; r < tokens; r++){
        for_direction(d) temp[r][d]=0;
    }

    int start=0, end=-1, separator=0, dir=0, steps=0, row=0;
    std::string dir_string, steps_string;
    
    // For every token,
    for( int token = 0; token < tokens; token++){
        // Begin just after the previous token ended,
        start=end+1;
        end=start;
        // find the end of the token and the colon separator,
        while( no_whitespace.substr(end,1) != ";"){
            if( ":" == no_whitespace.substr(end,1) ) separator=end;
            end++;
        }
        if( separator < start ){ // We didn't find a colon before the next semicolon!
            QDPIO::cout << "Paths can only be constructed from semicolon-separated direction:steps paths." << std::endl;
            QDP_abort(EXIT_FAILURE);
        }
        
        // Extract the substrings,
        dir_string=no_whitespace.substr(start,separator-start);
        steps_string=no_whitespace.substr(separator+1,end-separator-1);
        // turn them into integers,
        dir=std::stoi(dir_string);  
        steps=std::stoi(steps_string);
        // put them into the temporary array
        temp[row][dir]=steps;
        // and move on to the next row in the array.
        row++;
    }

    compressed=path_compress(temp);
    dx=path_expand(compressed);
    displacement=disp(compressed);
    zero_path=(compressed.nrows() == 0);


    END_CODE();
}

std::string Path::short_string(){
    return multi2d_int_to_string(compressed);
}
std::string Path::long_string(){
    return multi2d_int_to_string(dx);
}
