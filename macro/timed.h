#ifndef _HADRON_PDFS_MACRO_TIMED
#define _HADRON_PDFS_MACRO_TIMED

#include "standard_includes.h"
#include <time.h>
#include <string>

// Use only after Chroma::initialize or the QDPIO will segfault.
#define timed(msg) 	for(Timer MACRO_CLOCK_TIMER((msg)); MACRO_CLOCK_TIMER.loop; MACRO_CLOCK_TIMER.loop = false)
// This macro creates a one-time-through for loop that gets optimized away except for the desired side effects
// of starting and stopping (and reporting) a stopwatch.

// We accomplish this by creating a Timer.
class Timer{
public:
    bool loop;
    // Upon creation, start a stopwatch.
    Timer(std::string msg = "") {
        message=msg;
        QDPIO::cout << "[ " << message << " ]" << std::endl; 
        loop=true; 
        swatch.reset();
        swatch.start();
    };
    // Upon destruction, stop the clock and report the time.
    ~Timer(){
        swatch.stop();
        QDPIO::cout << "[ " << message << " : " << swatch.getTimeInSeconds() << " seconds ]" << std::endl;
    };
private:
    StopWatch swatch;
    std::string message;
};

// Example usage:
// timed("whole loop") for(int i = 0; i<100; i++){ ... }                    // Times the whole loop.
// for(int i=0; i<100; i++) timed("iteration "+int_to_string(i)){ ... }     // Times each iteration.
// timed("some code block") { ... }                                         // Times the code block.

// It is composable.
// timed("whole loop") for(int i = 0; i<100; i++) timed("i="+int_to_string(i)) for(int j = 0; j<1000; j++) timed("j="+int_to_string(j)){ j+=100; }

#endif /* end of include guard: _HADRON_PDFS_MACRO_TIMED */
