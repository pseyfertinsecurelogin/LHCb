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
#ifndef UTERRORDECODING_H
#define UTERRORDECODING_H 1

//===========================================
// Include files
// from Gaudi
#include "GaudiAlg/Consumer.h"
#include "UTDecodingBaseAlg.h"


/** @class UTErrorDecoding UTErrorDecoding.h public/UTErrorDecoding.h
 *
 *
 *  @author A Beiter (based on code by Mathias Knecht, M Needham, S Ponce)
 *  @date   2018-09-04
 */

class UTErrorDecoding : public Gaudi::Functional::Consumer<void(const LHCb::RawEvent&),
                                                           Gaudi::Functional::Traits::BaseClass_t<UTDecodingBaseAlg>> {
public:
  /// Standard constructor
  UTErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm execution
  void operator()(const LHCb::RawEvent&) const override;

private:

  Gaudi::Property<bool> m_PrintErrorInfo { this, "PrintErrorInfo", false };

};
#endif // UTERRORDECODING_H
