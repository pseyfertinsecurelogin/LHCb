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

#undef NDEBUG

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestSelection
#include "SOAContainer/SOAContainer.h"  // for Container
#include "SOAContainer/SOASkin.h"       // for SOASkinCreatorSimple<>::type
#include "SOAContainer/SOAView.h"       // for _View<>::reference, _View
#include "SOAExtensions/ZipContainer.h" // for ZipContainer, semantic_zip
#include "SOAExtensions/ZipSelection.h" // for SelectionView, SelectionVi...
#include <boost/test/unit_test.hpp>
// #include <boost/test/execution_monitor.hpp>
#include <memory>  // for allocator, allocator_trait...
#include <string>  // for operator<<, string
#include <utility> // for move
#include <vector>  // for vector

#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAUtils.h"
#include "SOAContainer/SOAView.h"

struct track {
  float                x;
  float                y;
  float                z;
  float                tx;
  float                ty;
  friend bool          operator==( const track& lhs, const track& rhs ) { return &lhs == &rhs; }
  friend bool          operator!=( const track& lhs, const track& rhs ) { return &lhs != &rhs; }
  friend std::ostream& operator<<( std::ostream& stream, const track& t ) {
    char buf[100];
    snprintf( buf, 99, "track at (%#8.2f, %#8.2f, %#8.2f)", t.x, t.y, t.z );
    stream << buf;
    return stream;
  }
};

SOAFIELD_TRIVIAL( f_track, accessor_track, track );
SOASKIN_TRIVIAL( s_track, f_track );

struct fitres {
  float                px;
  float                py;
  float                pz;
  int                  q;
  friend bool          operator==( const fitres& lhs, const fitres& rhs ) { return &lhs == &rhs; }
  friend bool          operator!=( const fitres& lhs, const fitres& rhs ) { return &lhs != &rhs; }
  friend std::ostream& operator<<( std::ostream& stream, const fitres& t ) {
    char buf[100];
    snprintf( buf, 99, "momentum = (%#8.2f, %#8.2f, %#8.2f), charge %d", t.px, t.py, t.pz, t.q );
    stream << buf;
    return stream;
  }
};

SOAFIELD_TRIVIAL( f_fitres, accessor_fitres, fitres );
SOASKIN_TRIVIAL( s_fitres, f_fitres );

struct fitqual {
  float       chi2;
  int         dof;
  friend bool operator==( const fitqual& lhs, const fitqual& rhs ) {
    return lhs.dof == rhs.dof && ( ( std::abs( lhs.chi2 - rhs.chi2 ) / ( lhs.chi2 + rhs.chi2 ) < 0.01 ) ||
                                   ( std::abs( lhs.chi2 ) < 0.01 && std::abs( rhs.chi2 ) < 0.01 ) );
  }
  friend std::ostream& operator<<( std::ostream& stream, const fitqual& t ) {
    char buf[100];
    snprintf( buf, 99, "chi2/dof = %#8.2f/%d = %#8.4f", t.chi2, t.dof, t.chi2 / ( (float)t.dof ) );
    stream << buf;
    return stream;
  }
};

SOAFIELD_TRIVIAL( f_fitqual, accessor_fitqual, fitqual );
SOASKIN_TRIVIAL( s_fitqual, f_fitqual );

SOASKIN_TRIVIAL( s_track_with_fitres, f_track, f_fitres );
SOASKIN_TRIVIAL( s_track_with_fitqual, f_track, f_fitqual );
SOASKIN_TRIVIAL( s_track_with_fitres_and_fitqual, f_track, f_fitres, f_fitqual );

/// pythonic sugar
#include "range/v3/all.hpp" // IWYU pragma: keep
// IWYU pragma : no_include <range/v3/view/indices.hpp>
// IWYU pragma : no_include <range/v3/view/take_exactly.hpp>
auto range = ranges::view::indices;
/// end of sugar

BOOST_AUTO_TEST_CASE( basic_zip_and_selection_operations ) {
  Zipping::ZipContainer<SOA::Container<std::vector, s_track>>   foo1;
  Zipping::ZipContainer<SOA::Container<std::vector, s_track>>   foo1_alt;
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>>  foo2( foo1.zipIdentifier() );
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitqual>> foo3( foo1.zipIdentifier() );
  for ( auto i : range( 42 ) ) {
    track t{i * 100.f, i * 2.f, ( 42 - i ) * 100.f, 0.f, 0.f};
    foo1.push_back( t );
    foo2.push_back( fitres{0.f, 0.f, i * 100.f, i} );
    foo3.push_back( fitqual{0.f, i} );
  }

  BOOST_CHECK_THROW( Zipping::semantic_zip<s_track_with_fitres>( foo1_alt, foo2 ), Zipping::IncompatibleZipException );
  auto                  track_with_momentum = Zipping::semantic_zip<s_track_with_fitres>( foo1, foo2 );
  [[maybe_unused]] auto full_track = Zipping::semantic_zip<s_track_with_fitres_and_fitqual>( foo1, foo2, foo3 );
  [[maybe_unused]] auto another_full_track =
      Zipping::semantic_zip<s_track_with_fitres_and_fitqual>( track_with_momentum, foo3 );
  [[maybe_unused]] auto yet_another_full_track = Zipping::semantic_zip<s_track_with_fitres_and_fitqual>( full_track );

  BOOST_CHECK_EQUAL( full_track[0], ( *full_track.begin() ) );

  auto exported_selection =
      Zipping::makeSelection( &track_with_momentum, []( auto i ) { return 0 == i.accessor_fitres().q % 2; } );

  Zipping::SelectionView<decltype( track_with_momentum )> selected_tracks{&track_with_momentum, exported_selection};

  Zipping::SelectionView<decltype( full_track )> selected_full_tracks( &full_track, exported_selection );

  auto crosscheck = selected_full_tracks.export_selection();

  BOOST_CHECK( crosscheck == exported_selection );

  [[maybe_unused]] auto refined =
      selected_full_tracks.select( []( const auto fulltrack ) { return fulltrack.accessor_fitqual().chi2 < 3; } );

  BOOST_CHECK_EQUAL( selected_tracks.size(), track_with_momentum.size() / 2 );

  BOOST_CHECK_EQUAL( *selected_tracks.begin(), selected_tracks.front() );
  BOOST_CHECK_EQUAL( *( selected_tracks.end() - 1 ), selected_tracks.back() );

  int keep_track = 0;
  for ( auto it = selected_full_tracks.begin(); it != selected_full_tracks.end(); ++it ) {
    BOOST_CHECK_EQUAL( ( *it ).accessor_fitres().q, 2 * keep_track );
    BOOST_CHECK_EQUAL( it - selected_full_tracks.begin(), keep_track );

    BOOST_CHECK( ( *it ) == selected_full_tracks[keep_track] );
    BOOST_CHECK_EQUAL( *it, selected_full_tracks[keep_track] );
    BOOST_CHECK_EQUAL( selected_full_tracks[keep_track], selected_full_tracks[keep_track] );
    keep_track += 1;
  }
  keep_track = 0;
  for ( auto proxy : selected_tracks ) { BOOST_CHECK_EQUAL( proxy, selected_tracks[keep_track++] ); }
  BOOST_CHECK( std::all_of( track_with_momentum.begin(), track_with_momentum.end(), []( auto t ) {
    return std::abs( t.accessor_track().x + t.accessor_track().z - 4200.f ) < 50.f;
  } ) );

  int sum_of_Q = 0;
  for ( size_t i = 0; i < selected_tracks.size(); ++i ) sum_of_Q += selected_tracks[i].accessor_fitres().q;

  std::for_each( selected_tracks.begin(), selected_tracks.end(),
                 [&sum_of_Q]( auto t ) { sum_of_Q -= t.accessor_fitres().q; } );

  BOOST_CHECK_EQUAL( sum_of_Q, 0 );

  BOOST_CHECK_EQUAL( std::count( track_with_momentum.begin(), track_with_momentum.end(), selected_tracks[2] ), 1 );

  BOOST_CHECK_EQUAL( std::count_if( track_with_momentum.begin(), track_with_momentum.end(),
                                    []( auto t ) { return t.accessor_fitres().q % 2 == 0; } ),
                     selected_tracks.size() );

  BOOST_CHECK( std::is_sorted( track_with_momentum.begin(), track_with_momentum.end(),
                               []( auto t1, auto t2 ) { return t1.accessor_fitres().py < t2.accessor_fitres().py; } ) );
}

BOOST_AUTO_TEST_CASE( stlalgs ) {
  Zipping::ZipContainer<SOA::Container<std::vector, s_track>>   foo1;
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>>  foo2( foo1.zipIdentifier() );
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitqual>> foo3( foo1.zipIdentifier() );
  for ( auto i : range( 42 ) ) {
    track t{i * 100.f, i * 2.f, ( 42 - i ) * 100.f, 0.f, 0.f};
    foo1.push_back( t );
    foo2.push_back( fitres{0.f, 0.f, i * 100.f, i} );
    foo3.push_back( fitqual{0.f, i} );
  }

  auto                         foo_all = Zipping::semantic_zip<s_track_with_fitres_and_fitqual>( foo1, foo2, foo3 );
  Zipping::ExportedSelection<> e =
      Zipping::makeSelection( &foo_all, []( auto f ) { return f.accessor_fitres().q % 2 == 0; } );
  Zipping::SelectionView foo_sel( &foo_all, e );

  auto found = std::find( foo_all.begin(), foo_all.end(), foo_all[3] );
  BOOST_CHECK_NE( found, foo_all.end() );

  auto founds = std::find( foo_sel.begin(), foo_sel.end(), foo_sel[2] );
  BOOST_CHECK( founds != foo_sel.end() );

  found = std::find( foo_all.begin(), foo_all.end(), foo_sel[2] );
  BOOST_CHECK_NE( found, foo_all.end() );

  for ( auto selected_element : foo_sel ) {
    found = std::find( foo_all.begin(), foo_all.end(), selected_element );
    BOOST_CHECK_NE( found, foo_all.end() );
  }
  bool prev = true;
  bool init = false;
  for ( auto element : foo_all ) {
    bool found_ = ( foo_sel.end() == std::find( foo_sel.begin(), foo_sel.end(), element ) );
    if ( !init ) prev = !found_;
    BOOST_CHECK_NE( found_, prev );
    prev = found_;
  }
}

BOOST_AUTO_TEST_CASE( set_operations ) {
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>> foo2;
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>> foo2_alt;
  for ( auto i : range( 42 ) ) {
    foo2.push_back( fitres{0.f, 0.f, i * 100.f, i} );
    foo2_alt.push_back( fitres{0.f, 0.f, i * 100.f, i} );
  }

  Zipping::ExportedSelection<> sel1 =
      Zipping::makeSelection( &foo2, []( auto i ) { return 0 == i.accessor_fitres().q % 2; } );
  Zipping::ExportedSelection<> sel2 =
      Zipping::makeSelection( &foo2, []( auto i ) { return 0 == i.accessor_fitres().q % 3; } );
  Zipping::ExportedSelection<> sel1_alt =
      Zipping::makeSelection( &foo2_alt, []( auto i ) { return 0 == i.accessor_fitres().q % 2; } );

  auto commahider = [&]( auto sel ) { return Zipping::SelectionView{&foo2, sel}; };

  BOOST_CHECK_THROW( commahider( sel1_alt ), GaudiException );

  Zipping::SelectionView tmp( &foo2, sel1 );

  auto union_       = set_union( sel1, sel2 );
  auto intersection = set_intersection( sel1, sel2 );
  auto symdiff      = set_symmetric_difference( sel1, sel2 );

  auto commahider2 = [&]() { return set_union( sel2, sel1_alt ); };

  BOOST_CHECK_THROW( commahider2(), GaudiException );
  BOOST_CHECK( includes( sel1, intersection ) );
}
