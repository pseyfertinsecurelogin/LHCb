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
#ifndef HLTSELREPORTSWRITER_H
#define HLTSELREPORTSWRITER_H 1

#include "Event/HltObjectSummary.h"
#include "Event/HltDecReports.h"
#include "Event/HltSelReports.h"
#include "Event/RawEvent.h"

#include "Kernel/IANNSvc.h"
#include "Kernel/IIndexedANNSvc.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class HltSelReportsWriter HltSelReportsWriter.h
 *
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-07-25
 *
 *  Algorithm to convert HltSelReports and HltObjectSummarys containers on TES to HltSelCandidates Raw Bank
 *
 */
class HltSelReportsWriter : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

  enum HeaderIDs { kVersionNumber=11 };


  enum SourceIDs { kSourceID_Dummy=0,
		   kSourceID_Hlt=kSourceID_Dummy,
		   kSourceID_Hlt1=1,
		   kSourceID_Hlt2=2,
		   kSourceID_Max=7,
		   kSourceID_BitShift=13,
		   kSourceID_MinorMask=0x1FFF,
		   kSourceID_MajorMask=0xE000
  };
private:

  /// location of input H
  Gaudi::Property<std::string> m_inputHltSelReportsLocation { this, "InputHltSelReportsLocation",  LHCb::HltSelReportsLocation::Default };

  /// location of input HltDecReports, used to get the TCK, see UseTCK
  Gaudi::Property<std::string> m_inputHltDecReportsLocation { this, "InputHltDecReportsLocation",  LHCb::HltDecReportsLocation::Default };

  /// whether to try to use the TCK and the TCKANNSvc
  Gaudi::Property<bool> m_useTCK { this, "UseTCK",  false };

  /// location of output
  Gaudi::Property<std::string> m_outputRawEventLocation { this, "OutputRawEventLocation", LHCb::RawEventLocation::Default };

  /// SourceID to insert in the bank header (0-7)
  Gaudi::Property<int> m_sourceID { this, "SourceID", kSourceID_Dummy };

  /// HltANNSvc for making selection names to int selection ID
  SmartIF<IANNSvc> m_hltANNSvc;
  /// TCKANNSvc to be used in case the
  SmartIF<IIndexedANNSvc> m_tckANNSvc;

  using NameToNumberMap = std::map<std::string, unsigned int>;
  mutable std::map<std::pair<unsigned int, Gaudi::StringKey>, NameToNumberMap> m_infoTable;

  const NameToNumberMap& tckANNSvcMap(unsigned int tck, const Gaudi::StringKey& major) const;

  inline std::optional<int> optionalValue(const Gaudi::StringKey& major, const std::string& key) const {
    if ( const auto p = m_hltANNSvc->value(major, key); p ) {
      return p->second;
    } else {
      return {};
    }
  }

  inline std::optional<int> optionalFind(const NameToNumberMap& map, const std::string& key) const {
    if (auto i = map.find(key); i != std::end(map)) {
      return i->second;
    } else {
      return {};
    }
  }

};


#endif // HLTSELREPORTSWRITER_H
