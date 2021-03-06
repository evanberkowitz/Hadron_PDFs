#ifndef _HADRON_PDFS_STANDARD_INCLUDES
#define _HADRON_PDFS_STANDARD_INCLUDES


#ifdef _OPENMP
#include <omp.h>
#endif

//chroma stuff
#include "qdp.h"
#include "chroma.h"

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdio>

#include <stdlib.h>
#include <sys/time.h>
#include <cmath>
#include <limits>

#include "macro/lattice.h"
#include "macro/timed.h"
#include "utility/banner.h"
#include "utility/int_to_string.h"
#include "utility/vector_to_string.h"
#include "utility/sign.h"
#include "utility/file_exists.h"

#include "io/xml.h"
#include "io/trim_string.h"

#endif // _HADRON_PDFS_STANDARD_INCLUDES