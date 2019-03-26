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
#ifndef COMPONENT_COMPARECALOFUTUREDIGITS_H
#define COMPONENT_COMPARECALOFUTUREDIGITS_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/CaloDigit.h"

/** @class CompareCaloFutureDigits CompareCaloFutureDigits.h component/CompareCaloFutureDigits.h
 *  Compare the containers of CaloDigits
 *
 *  @author Olivier Callot
 *  @date   2003-11-18
 */
class CompareCaloFutureDigits final : public GaudiAlgorithm {
public:
  /// Standard constructor
  CompareCaloFutureDigits( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  void compareContainers( const LHCb::CaloDigits& dig1, const LHCb::CaloDigits& dig2, double tol ) const;

  std::string m_extension;
  bool        m_packedRawBuffer;
};
#endif // COMPONENT_COMPARECALOFUTUREDIGITS_H
