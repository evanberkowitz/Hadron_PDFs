#include "standard_includes.h"
#include "include/SpinBasis.h"

std::string inline to_string(const SPIN_BASIS &basis){
    switch(basis){
        case SPIN_BASIS::DIRAC_PAULI:       return "DIRAC_PAULI";               break;
        case SPIN_BASIS::DEGRAND_ROSSI:     return "DEGRAND_ROSSI";             break;
        
        default:                            return "unknown spin basis";
    };
}

void spin_basis_transform(LatticePropagator& prop, const SPIN_BASIS new_basis, const SPIN_BASIS old_basis){
    SpinMatrix rotator;
         if ( new_basis     == old_basis){ return; }
    // Chroma has a built-in for converting from Dirac-Pauli to Degrand-Rossi
    else if ( SPIN_BASIS::DIRAC_PAULI   == new_basis    &&  SPIN_BASIS::DEGRAND_ROSSI   == old_basis    ){ rotator = adj(PauliToDRMat());   }
    else if ( SPIN_BASIS::DEGRAND_ROSSI == new_basis    &&  SPIN_BASIS::DIRAC_PAULI     == old_basis    ){ rotator = PauliToDRMat();        }
    
    else    {   
        QDPIO::cout << "Spin basis transformation is unknown." << std::endl;
        QDP_abort(EXIT_FAILURE);
    }

    LatticePropagator temp;
    temp = rotator * prop * adj(rotator);
    prop = temp;
    return;
}
