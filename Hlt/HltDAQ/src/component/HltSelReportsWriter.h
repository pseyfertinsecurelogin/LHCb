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

#include "Event/HltDecReports.h"
#include "Event/HltObjectSummary.h"
#include "Event/HltSelReports.h"
#include "Event/RawEvent.h"

#include "Kernel/IANNSvc.h"
#include "Kernel/IIndexedANNSvc.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include "GaudiAlg/Consumer.h"

/** @class HltSelReportsWriter HltSelReportsWriter.h
 *
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-07-25
 *
 *  Algorithm to convert HltSelReports and HltObjectSummarys containers on TES to HltSelCandidates Raw Bank
 *
 */
using HltSelReportsWriterBase_t =
    Gaudi::Functional::Consumer<void( LHCb::HltDecReports const&, LHCb::HltSelReports const&,
                                      LHCb::HltObjectSummary::Container const&, LHCb::RawEvent const& )>;

class HltSelReportsWriter : public HltSelReportsWriterBase_t {
public:
  HltSelReportsWriter( const std::string& name, ISvcLocator* pSvcLocator )
      : HltSelReportsWriterBase_t( name, pSvcLocator,
                                   {KeyValue{"DecReports", LHCb::HltDecReportsLocation::Default},
                                    KeyValue{"SelReports", LHCb::HltSelReportsLocation::Default},
                                    KeyValue{"ObjectSummaries", LHCb::HltObjectSummaryLocation::Default},
                                    KeyValue{"RawEvent", LHCb::RawEventLocation::Default}} ) {}

  StatusCode initialize() override; ///< Algorithm initialization
  void       operator()( LHCb::HltDecReports const& decreps, LHCb::HltSelReports const& selreps,
                   LHCb::HltObjectSummary::Container const& objectSummaries,
                   LHCb::RawEvent const&                    rawevt ) const override; ///< Algorithm execution

  enum HeaderIDs { kVersionNumber = 11 };

  enum SourceIDs {
    kSourceID_Dummy     = 0,
    kSourceID_Hlt       = kSourceID_Dummy,
    kSourceID_Hlt1      = 1,
    kSourceID_Hlt2      = 2,
    kSourceID_Max       = 7,
    kSourceID_BitShift  = 13,
    kSourceID_MinorMask = 0x1FFF,
    kSourceID_MajorMask = 0xE000
  };

private:
  /// whether to try to use the TCK and the TCKANNSvc
  Gaudi::Property<bool> m_useTCK{this, "UseTCK", false};

  /// SourceID to insert in the bank header (0-7)
  Gaudi::Property<int> m_sourceID{this, "SourceID", kSourceID_Dummy};

  /// HltANNSvc for making selection names to int selection ID
  SmartIF<IANNSvc> m_hltANNSvc;
  /// TCKANNSvc to be used in case the
  SmartIF<IIndexedANNSvc> m_tckANNSvc;

  using NameToNumberMap = std::map<std::string, unsigned int>;
  mutable std::map<std::pair<unsigned int, Gaudi::StringKey>, NameToNumberMap> m_infoTable;

  const NameToNumberMap& tckANNSvcMap( unsigned int tck, const Gaudi::StringKey& major ) const;

  inline std::optional<int> optionalValue( const Gaudi::StringKey& major, const std::string& key ) const {
    if ( const auto p = m_hltANNSvc->value( major, key ); p ) {
      return p->second;
    } else {
      return std::nullopt;
    }
  }

  inline std::optional<int> optionalFind( const NameToNumberMap& map, const std::string& key ) const {
    if ( auto i = map.find( key ); i != std::end( map ) ) {
      return i->second;
    } else {
      return std::nullopt;
    }
  }
};

#endif // HLTSELREPORTSWRITER_H
