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
// $Id: RawBankReadoutStatusFilter.h,v 1.1 2007/12/05 13:59:18 odescham Exp $
#ifndef COMPONENT_RAWBANKREADOUTSTATUSFILTER_H
#define COMPONENT_RAWBANKREADOUTSTATUSFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/RawBank.h"
#include "Event/RawBankReadoutStatus.h"

/** @class RawBankReadoutStatusFilter RawBankReadoutStatusFilter.h component/RawBankReadoutStatusFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-05
 */
class RawBankReadoutStatusFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawBankReadoutStatusFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawBankReadoutStatusFilter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:

  mutable Gaudi::Accumulators::Counter<> m_acceptedEventCount{ this, "Accepted events" };
  mutable Gaudi::Accumulators::Counter<> m_rejectedEventCount{ this, "Rejected events" };

  int m_type;
  long m_mask;
  bool m_invert;
};
#endif // COMPONENT_RAWBANKREADOUTSTATUSFILTER_H
