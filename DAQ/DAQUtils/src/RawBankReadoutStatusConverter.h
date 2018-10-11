/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// $Id: $
#ifndef RAWBANKREADOUTSTATUSCONVERTER_H
#define RAWBANKREADOUTSTATUSCONVERTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class RawBankReadoutStatusConverter RawBankReadoutStatusConverter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2012-03-30
 */
class RawBankReadoutStatusConverter : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawBankReadoutStatusConverter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawBankReadoutStatusConverter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:
  std::string reason(int status,std::string typeName);
  std::vector<std::string> m_types;
  std::vector<std::string> m_flags;
  std::string m_system;
  unsigned int m_mask;
};
#endif // RAWBANKREADOUTSTATUSCONVERTER_H
