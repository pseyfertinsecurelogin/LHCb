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
#ifndef UNPACKCALOHYPO_H
#define UNPACKCALOHYPO_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/CaloHypo.h"
#include "Event/PackedCaloHypo.h"

/** @brief Unpack a PackedCaloHypo container to CaloHypos.
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackCaloHypo : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  StatusCode execute() override;

private:
  DataObjectReadHandle<LHCb::PackedCaloHypos> m_packedHypos{this, "InputName", LHCb::PackedCaloHypoLocation::Electrons};
  DataObjectWriteHandle<LHCb::CaloHypos>      m_hypos{this, "OutputName", LHCb::CaloHypoLocation::Electrons};
  Gaudi::Property<bool>                       m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                       "Flag to turn on the creation of output, even when input is missing"};
};

#endif // UNPACKCALOHYPO_H
