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
#include "Event/TrackAfterFit.h"

LHCb::Event::experimental::TrackExtension::Hits::Hits( const std::vector<LHCb::LHCbID>& container )
    : m_hits( container.begin(), container.end() )
{
}

LHCb::Event::experimental::TrackExtension::Hits::Hits( const LHCb::span<const LHCb::LHCbID> container )
    : m_hits( container.begin(), container.end() )
{
}

LHCb::Event::experimental::TrackExtension::AdditionalStates::
AdditionalStates( std::vector<LHCb::State> && states , ::experimental::Tag::State::AssumeFiltered_tag )
  : m_moreStates( std::forward< std::vector<LHCb::State> > ( states ) )
{

}

LHCb::Event::experimental::TrackExtension::AdditionalStates::
AdditionalStates( std::vector<LHCb::State> && states , ::experimental::Tag::State::AssumeUnfiltered_tag )
  : m_moreStates( std::forward< std::vector<LHCb::State> > ( states ) )
{
  auto rm = [this]( const LHCb::State::Location location ) {
    auto iter = std::find_if( this->m_moreStates.begin(), this->m_moreStates.end(),
                              [&]( const auto& s ) { return s.checkLocation( location ); } );
    if ( iter != m_moreStates.end() ) {
      this->m_moreStates.erase( iter );
    }
  };

  rm( LHCb::State::BegRich1 );
  rm( LHCb::State::EndRich1 );
  rm( LHCb::State::BegRich2 );
}

LHCb::Event::experimental::TrackExtension::Rich1States::
Rich1States( LHCb::State&& Beg, LHCb::State&& End )
    : m_BegRich1( std::move( Beg ) ), m_EndRich1( std::move( End ) )
{
}

LHCb::Event::experimental::TrackExtension::Rich1States::
Rich1States( std::vector<LHCb::State> & states )
{
  auto rearrange = [&]( const LHCb::State::Location location, LHCb::State& output ) {
    auto iter = std::find_if( states.begin(), states.end(),
                              [&]( const auto& s ) { return s.checkLocation( location ); } );
    if ( iter != states.end() ) {
      output = std::move( *iter );
      states.erase( iter );
    }
  };

  rearrange( LHCb::State::BegRich1, m_BegRich1 );
  rearrange( LHCb::State::EndRich1, m_EndRich1 );
}

LHCb::Event::experimental::TrackExtension::Rich2States::
Rich2States( std::vector<LHCb::State> & states )
{
  auto rearrange = [&]( const LHCb::State::Location location, LHCb::State& output ) {
    auto iter = std::find_if( states.begin(), states.end(),
                              [&]( const auto& s ) { return s.checkLocation( location ); } );
    if ( iter != states.end() ) {
      output = std::move( *iter );
      states.erase( iter );
    }
  };

  rearrange( LHCb::State::BegRich2, m_BegRich2 );
}
