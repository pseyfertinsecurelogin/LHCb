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
#ifndef STERRORDECODING_H
#define STERRORDECODING_H 1

//===========================================
// Include files
// from Gaudi
#include "GaudiAlg/Consumer.h"
#include "STDecodingBaseAlg.h"


/** @class STErrorDecoding STErrorDecoding.h public/STErrorDecoding.h
 *
 *
 *  @author Mathias Knecht, M Needham, S Ponce
 *  @date   2007-09-11 (2008-06)
 */

class STErrorDecoding : public Gaudi::Functional::Consumer<void(const LHCb::RawEvent&),
                                                           Gaudi::Functional::Traits::BaseClass_t<STDecodingBaseAlg>> {
public:
  /// Standard constructor
  STErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm execution
  void operator()(const LHCb::RawEvent&) const override;

private:

  Gaudi::Property<bool> m_PrintErrorInfo { this, "PrintErrorInfo", false };

};
#endif // STERRORDECODING_H
