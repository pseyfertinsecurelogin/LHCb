// $Id: LbAppInitFactory.cpp,v 1.1 2010-02-12 14:00:30 gcorti Exp $
// Include files 
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "Kernel/LbAppInit.h"

// Put LbAppInit base class factory in this library, to avoid having it in
// LHCbKernel component library. 
// It must be present otherwise the class cannot be instanciated
DECLARE_ALGORITHM_FACTORY( LbAppInit )

