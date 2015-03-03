// $Id: STDetDict.h,v 1.3 2008-08-04 07:23:56 mneedham Exp $
// ============================================================================
#ifndef STDET_STDETDICT_H 
#define STDET_STDETDICT_H 1

#include "STDet/DeITBox.h"
#include "STDet/DeITDetector.h"
#include "STDet/DeITLadder.h"
#include "STDet/DeITLayer.h"
#include "STDet/DeITSector.h"
#include "STDet/DeITStation.h"
#include "STDet/DeSTBaseElement.h"
#include "STDet/DeSTDetector.h"
#include "STDet/DeSTLayer.h"
#include "STDet/DeSTSector.h"
#include "STDet/DeSTStation.h"
#include "STDet/DeTTDetector.h"
#include "STDet/DeTTHalfModule.h"
#include "STDet/DeTTLayer.h"
#include "STDet/DeTTSector.h"
#include "STDet/DeTTStation.h"
#include "STDet/DeTTSensor.h"
#include "STDet/DeITSensor.h"
#include "STDet/DeSTSensor.h"

// Needed to resolve forward declaration when making dictiobary
#include "Kernel/Trajectory.h"

namespace {
  struct _Instantiations {
    // begin instantiations
    std::vector<DeTTStation*> m_ttStation_vector;
    std::vector<DeITStation*> m_itStation_vector;
    std::vector<DeITSector*> m_itsector_vector;
    std::vector<DeITSensor*> m_itsensor_vector;
    std::vector<DeTTSensor*> m_ttsensor_vector;
    std::map<unsigned int,DeSTSector::Status> m_unsigned_int_Status_map; 
    std::pair<unsigned int,DeSTSector::Status> m_unsigned_int_Status_pair;
  } ;
}

// ============================================================================
#endif // STDET_STDETDICT_H
// ============================================================================

