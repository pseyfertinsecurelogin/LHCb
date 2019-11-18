/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Event/GeneratePrFittedForwardTracks.h"
#include "GaudiAlg/Producer.h"

using namespace LHCb::Pr::Fitted::Forward;
using base_t = Gaudi::Functional::Producer<Tracks()>;

struct ProducePrFittedForwardTracks final : public base_t {
  ProducePrFittedForwardTracks( std::string const& name, ISvcLocator* svcLoc )
      : base_t( name, svcLoc, KeyValue{"Output", ""} ) {}

  Tracks operator()() const override { return generate_tracks( m_nTracks, ( m_eventCount++ ).value() ); }

private:
  mutable Gaudi::Accumulators::Counter<unsigned int> m_eventCount{this, "Event"};
  Gaudi::Property<std::size_t> m_nTracks{this, "NumberToGenerate", 100, "Number of objects to generate"};
};

DECLARE_COMPONENT( ProducePrFittedForwardTracks )