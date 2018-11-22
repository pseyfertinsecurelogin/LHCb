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
#ifndef HLTVERTEXREPORTSWRITER_H
#define HLTVERTEXREPORTSWRITER_H 1

#include "Event/HltVertexReports.h"
#include "Event/RawEvent.h"
#include "Kernel/IANNSvc.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include <optional>


/** @class HltvertexReportsWriter HltvertexReportsWriter.h
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-05
 *
 *  Algorithm to translate HltSummary  into HltVertexReports
 *
 */
class HltVertexReportsWriter : public GaudiAlgorithm {
public:

  enum HeaderIDs { kVersionNumber=2 };


  enum SourceIDs { kSourceID_Dummy=0,
		   kSourceID_Hlt=kSourceID_Dummy,
		   kSourceID_Hlt1=1,
		   kSourceID_Hlt2=2,
		   kSourceID_Max=7,
		   kSourceID_BitShift=13,
		   kSourceID_MinorMask=0x1FFF,
		   kSourceID_MajorMask=0xE000
  };


  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  ~HltVertexReportsWriter( ) override = default; ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  // ----------------------- methods

  std::optional<int> selectionNameToInt(const IANNSvc& ann, const std::string& name) const;

  // ----------------------- data members

  /// location of input
  Gaudi::Property<std::string> m_inputHltVertexReportsLocation { this, "InputHltVertexReportsLocation",  LHCb::HltVertexReportsLocation::Default };

  /// location of output
  Gaudi::Property<std::string> m_outputRawEventLocation { this,  "OutputRawEventLocation", LHCb::RawEventLocation::Default };

  /// SourceID to insert in the bank header
  Gaudi::Property<int> m_sourceID { this, "SourceID", kSourceID_Dummy };

  /// HltANNSvc for making selection names to int selection ID
  SmartIF<IANNSvc> m_hltANNSvc;

};

#endif // HLTVERTEXREPORTSWRITER_H
