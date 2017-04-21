#ifndef _HADRON_PDFS_SPIN_BASIS
#define _HADRON_PDFS_SPIN_BASIS

enum class SPIN_BASIS{
    DIRAC_PAULI     = 0,
    DEGRAND_ROSSI   = 1,
    CHROMA          = 1,
    QDP             = 1
};


std::string inline to_string(const SPIN_BASIS &basis);

void spin_basis_transform(LatticePropagator& prop, const SPIN_BASIS new_basis, const SPIN_BASIS old_basis);

#endif /* end of include guard: _HADRON_PDFS_SPIN_BASIS */