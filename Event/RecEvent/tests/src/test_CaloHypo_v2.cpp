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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestTrackV2
#include <boost/test/unit_test.hpp>
#include <type_traits>

#include "Event/State.h"
#include "GaudiKernel/Property.h"

#include "Kernel/LHCbID.h"
#include <ostream>
#include <sstream>

#include "Event/CaloCluster.h"
#include "Event/CaloHypo_v2.h"

// The test fixture: Create a container of tracks
// This can be used as a common starting point for all tests

using namespace LHCb::Event::v2;
using Hypothesis = Enum::CaloHypo::Hypothesis;

struct ExampleCaloHypos {

  std::vector<CaloHypo> m_calohypos;

  ExampleCaloHypos() {

    BOOST_TEST_MESSAGE( "setup example calohypos" );

    LHCb::CaloDigit                       digit1;
    LHCb::CaloDigit                       digit2;
    std::vector<LHCb::CaloDigit>          digits{digit1, digit2};
    LHCb::CaloCluster                     cluster;
    std::vector<const LHCb::CaloCluster*> clusters{&cluster};
    LHCb::CaloPosition                    position;
    auto& calohypo = m_calohypos.emplace_back( Hypothesis::Pi0Merged, 1.0, position, digits, clusters );

    m_calohypos.push_back( calohypo );
  }

  ~ExampleCaloHypos() { BOOST_TEST_MESSAGE( "tear down example calohypos" ); }
};

BOOST_AUTO_TEST_CASE( test_calo_construction ) {

  LHCb::CaloPosition                    position;
  std::vector<LHCb::CaloDigit>          vec_digit;
  std::vector<const LHCb::CaloCluster*> vec_clusters;
  std::vector<const CaloHypo*>          vec_hypos;
  auto hypo1 = CaloHypo( Hypothesis::Pi0Merged, 0.5, position, vec_digit, vec_clusters, vec_hypos );
  BOOST_CHECK( hypo1.hypothesis() == Hypothesis::Pi0Merged );
  BOOST_CHECK( hypo1.likelihood() == 0.5 );

  auto hypo2 = CaloHypo( Hypothesis::Mip, 0.5, position, vec_digit, vec_clusters, vec_hypos );
}

BOOST_AUTO_TEST_CASE( test_more_calo_construction ) {

  auto calohypo = CaloHypo();

  calohypo.setHypothesis( Hypothesis::Pi0Merged );
  calohypo.setLikelihood( 1 );

  LHCb::CaloPosition position;
  calohypo.setPosition( position );

  LHCb::CaloDigit digit1;
  calohypo.reserveToDigits( 9 );
  calohypo.addToDigits( digit1 );

  LHCb::CaloCluster cluster;
  calohypo.setClusters( &cluster );

  CaloHypo hypo1;
  calohypo.reserveToHypos( 2 );
  calohypo.addToHypos( &hypo1 );
}
