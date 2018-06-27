#include "Event/TrackAfterFit.h"

experimental::TrackAfterFitExtension::Hits::Hits( const std::vector<LHCb::LHCbID>& container )
    : m_hits( container.begin(), container.end() )
{
}
experimental::TrackAfterFitExtension::Hits::Hits( const LHCb::span<const LHCb::LHCbID> container )
    : m_hits( container.begin(), container.end() )
{
}

experimental::TrackAfterFitExtension::AdditionalStates::AdditionalStates( LHCb::States::const_iterator it1,
                                                                          LHCb::States::const_iterator it2 )
    : m_moreStates( it1, it2 )
{

  auto rearrange =
      [this]( LHCb::State::Location location, LHCb::State& output ) {
        auto iter = std::find_if( this->m_moreStates.begin(), this->m_moreStates.end(),
                                  [&]( const LHCb::State* s ) { return s->checkLocation( location ); } );
        if ( iter != m_states.end() ) {
          auto retval = std::move( *iter );
          this->m_moreStates->erase( iter );
        }
      }

  rearrange( LHCb::State::BegRich1, m_BegRich1 );
  rearrange( LHCb::State::EndRich1, m_EndRich1 );
  rearrange( LHCb::State::BegRich2, m_BegRich2 );
}

