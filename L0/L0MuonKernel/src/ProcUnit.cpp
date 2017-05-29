//#include <time.h>
#include "L0MuonKernel/ProcUnit.h"

namespace xercesc_3_1 {
class DOMNode;
}  // namespace xercesc_3_1

/**
   Empty Constructor
*/
L0Muon::ProcUnit::ProcUnit() {  
}


/**
   Constructor
*/
L0Muon::ProcUnit::ProcUnit(DOMNode* pNode):L0MUnit(pNode) {
  
}

/**
   Destructor
*/
L0Muon::ProcUnit::~ProcUnit() {  
}

