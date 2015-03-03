
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/SvcFactory.h"

#include "MDF/MDFWriter.h"
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb,MDFWriter)

#include "MDF/MEPWriter.h"
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb,MEPWriter)

#include "MDF/MIFWriter.h"
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb,MIFWriter)

#include "MDF/RawDataCnvSvc.h"
DECLARE_NAMESPACE_SERVICE_FACTORY(LHCb,RawDataCnvSvc)

#include "MDF/RawDataWriter.h"
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb,RawDataWriter)
