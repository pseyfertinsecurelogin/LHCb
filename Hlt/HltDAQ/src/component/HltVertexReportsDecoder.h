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
#ifndef HLTVERTEXREPORTSDECODER_H 
#define HLTVERTEXREPORTSDECODER_H 1

#include "HltRawBankDecoderBase.h"

#include "Event/VertexBase.h"

/** @class HltvertexReportsDecoder HltvertexReportsDecoder.h
 *  
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-05
 *
 *  Algorithm to translate HltSummary  into HltVertexReports
 *
 */
class HltVertexReportsDecoder : public HltRawBankSplittingDecoder<LHCb::VertexBase::Container> {
public:
  enum HeaderIDs { kVersionNumber=2 };

  /// Standard constructor
  HltVertexReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  ///< Algorithm execution
  Gaudi::Functional::vector_of_optional_<LHCb::VertexBase::Container> operator()(const LHCb::RawEvent&) const override; 

private:
  std::vector<std::string> m_decode;               /// which containers to decode

};

#endif // HLTVERTEXREPORTSDECODER_H
