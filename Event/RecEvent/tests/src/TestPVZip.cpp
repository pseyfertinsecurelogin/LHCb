/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#include "Event/IterableRecVertices.h"
#include "Event/RecVertices.h"
#include "GaudiKernel/System.h"
#include "SOAExtensions/ZipUtils.h"
#include <iostream>
#include <typeinfo>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestPVs
#include <boost/test/unit_test.hpp>

[[nodiscard]] LHCb::Rec::PV::PVs make_PVs( std::vector<std::pair<int, float>>& tracks ) {
  LHCb::Rec::PV::PVs  retval( Zipping::generateZipIdentifier(), Zipping::generateZipIdentifier() );
  Gaudi::XYZPoint     pos{0.1, 0.2, 0.3};
  Gaudi::SymMatrix3x3 covmatrix;

  float chi2{42};
  int   ndof{23};

  retval.emplace_back( pos, covmatrix, LHCb::Event::v2::Track::Chi2PerDoF{chi2 / ndof, ndof}, tracks );

  return retval;
}

BOOST_AUTO_TEST_CASE( create_PVs ) {
  std::vector<std::pair<int, float>> tracks; // as used in the TBLVertexFinder

  tracks.push_back( std::make_pair( -4, 3.1f ) );
  tracks.push_back( std::make_pair( -2, 2.2f ) );
  tracks.push_back( std::make_pair( -1, 1.3f ) );
  tracks.push_back( std::make_pair( 0, 6.4f ) );
  tracks.push_back( std::make_pair( 3, 7.5f ) );
  tracks.push_back( std::make_pair( 14, 8.6f ) );
  tracks.push_back( std::make_pair( 34, 9.7f ) );
  tracks.push_back( std::make_pair( 74, 4.8f ) );

  auto pvs = make_PVs( tracks );

  static_assert( LHCb::Pr::is_zippable_v<decltype( pvs )> );
  static_assert( LHCb::Pr::is_zippable_v<LHCb::Rec::PV::PVs> );

  auto iterable_pvs = LHCb::Pr::make_zip( pvs );

  std::cout << "pvs are of type " << System::typeinfoName( typeid( iterable_pvs ) ) << '\n';
  // LHCb::Pr::Zip<(SIMDWrapper::InstructionSet)0,false,LHCb::Rec::PV::PVs>

  for ( auto const& pv : iterable_pvs.with<SIMDWrapper::InstructionSet::Scalar>() ) {
    //
    [[maybe_unused]] float x    = pv.x().cast();
    [[maybe_unused]] auto  pos  = pv.pos();
    [[maybe_unused]] auto  chi2 = pv.chi2perdof().chi2();
  }
  for ( auto const& pv : iterable_pvs ) {
    //
    [[maybe_unused]] auto x    = pv.x();
    [[maybe_unused]] auto pos  = pv.pos();
    [[maybe_unused]] auto chi2 = pv.chi2perdof().chi2();
  }
}
