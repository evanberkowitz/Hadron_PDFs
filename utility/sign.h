#ifndef _HADRON_PDFS_UTILITY_SIGN
#define _HADRON_PDFS_UTILITY_SIGN

int inline sign(const int &qty){
    return (0 < qty) - (qty < 0);    // A fancy, branch-free way of writing the sign function.
}

#endif /* end of include guard: _HADRON_PDFS_UTILITY_SIGN */
