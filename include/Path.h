#ifndef _PATH_H
#define _PATH_H

#include "standard_includes.h"

// A Path is a set of unit steps along lattice directions.
// Consecutive steps than are opposite one another are deleted.

class Path{
public:
    
    // There's a default constructor which creates a path that's zero steps.
    Path(); 
    
    // There's a copy constructor
    Path(const Path &p);
    
    // A path can be built from a 2-dimensional set of integers.
    // There can be any number of rows, and each row should be Nd columns wide---one integer for each spacetime direction.
    // Every entry but one must be zero (so that the path is along a lattice direction).
    // The absolute value of the nonzero entry can be more than one, but it will be put into a normal form with that many rows with just one ±1 instead.
    Path(const multi2d<int> &P);

    // A path can also be built from a string which describes it.
    // TODO: specify a format.
    Path(const std::string  &P);
    // You can ask for the displacement from the start of the path to the end of the path.
    multi1d<int> total_displacement(){ return displacement; };
    
    // You might want to reverse a path.
    Path reverse();
    
    // Some paths might be zero links long.  That is DIFFERENT from having total_displacement() == 0!
    bool is_nonzero(){ return !zero_path; };
    bool is_zero()   { return  zero_path; };
    
    // A path can be turned into a string:
    std::string short_string();
    std::string  long_string();

    // You can destroy a path.
   ~Path(){  };
protected:
    bool zero_path;
    multi2d<int> dx;
    multi2d<int> compressed;
    multi1d<int> displacement;
};


#endif /* end of include guard: _PATH_H */
