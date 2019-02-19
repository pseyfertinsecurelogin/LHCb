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

#include "Event/PackedCaloHypo.h"

/** @class UnpackCaloHypo UnpackCaloHypo.h
 *
 *  Unpack CaloHypos
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackCaloHypo : public GaudiAlgorithm {

public:
  /// Standard constructor
  UnpackCaloHypo( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  std::string m_inputName;
  std::string m_outputName;
  bool        m_alwaysOutput; ///< Flag to turn on the creation of output, even when input is missing
};

#endif // UNPACKCALOHYPO_H
