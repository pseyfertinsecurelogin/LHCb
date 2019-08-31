/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef UTDET_UTDETDICT_H
#define UTDET_UTDETDICT_H 1

#include "UTDet/DeUTBaseElement.h"
#include "UTDet/DeUTDetector.h"
#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTSector.h"
#include "UTDet/DeUTSensor.h"
#include "UTDet/DeUTStation.h"
#include "UTDet/DeUTStave.h"

// Needed to resolve forward declaration when making dictiobary
#include "Kernel/Trajectory.h"

namespace {
  struct _Instantiations {
    // begin instantiations
    std::vector<DeUTStation*>                   m_utStation_vector;
    std::vector<DeUTSensor*>                    m_utsensor_vector;
    std::map<unsigned int, DeUTSector::Status>  m_unsigned_int_Status_map;
    std::pair<unsigned int, DeUTSector::Status> m_unsigned_int_Status_pair;
  };
} // namespace

// ============================================================================
#endif // UTDET_UTDETDICT_H
