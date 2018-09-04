// $Id: UTDetDict.h,v 1.3 2008-08-04 07:23:56 mneedham Exp $
// ============================================================================
#ifndef UTDET_UTDETDICT_H 
#define UTDET_UTDETDICT_H 1

#include "UTDet/DeUTBaseElement.h"
#include "UTDet/DeUTDetector.h"
#include "UTDet/DeUTStation.h"
#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTModule.h"
#include "UTDet/DeUTSector.h"
#include "UTDet/DeUTSensor.h"

// Needed to resolve forward declaration when making dictiobary
#include "Kernel/Trajectory.h"

namespace {
  struct _Instantiations {
    // begin instantiations
    std::vector<DeUTStation*> m_utStation_vector;
    std::vector<DeUTSensor*> m_utsensor_vector;
    std::map<unsigned int,DeUTSector::Status> m_unsigned_int_Status_map; 
    std::pair<unsigned int,DeUTSector::Status> m_unsigned_int_Status_pair;
  } ;
}

// ============================================================================
#endif // UTDET_UTDETDICT_H
// ============================================================================

