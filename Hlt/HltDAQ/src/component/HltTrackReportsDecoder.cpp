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
// Include files
#include "boost/format.hpp"
#include <algorithm>
#include <numeric>

#include "Event/RawEvent.h"

// local
#include "HltTrackReportsDecoder.h"
#include "HltTrackReportsWriter.h"
#include "HltTrackingCoder.h"

#include "Event/Track.h"

using namespace LHCb;

namespace
{
int sourceID( const RawBank& bank )
{
     return bank.sourceID() >> HltTrackReportsWriter::kSourceID_BitShift;
}

constexpr struct cmp_sourceID_t {
    bool operator()(int id, const RawBank* bank) const { return id < sourceID( *bank ); }
    bool operator()(const RawBank* bank, int id) const { return sourceID( *bank ) < id; }
} cmp_sourceID{};

int seq( const RawBank& bank )
{
    return bank.sourceID() & HltTrackReportsWriter::kSourceID_MinorMask;
}
static const std::vector<std::string> DefaultLabels = { "Velo", "VeloTTHPT","ForwardHPT" };

template <typename StringContainer>
std::vector<std::string> prefix(const std::string& prefix, const StringContainer& strs ) {
    std::vector<std::string> vs; vs.reserve(strs.size());
    std::transform( strs.begin(), strs.end(),
                    std::back_inserter(vs),
                    [&](const std::string& s) {  return prefix + "/" + s; } );
    return vs;
}
}

//-----------------------------------------------------------------------------
// Implementation file for class : HltTrackReportsDecoder
//
// The actual decoding of tracks is delegated
// to the functions in HltTrackingCoder.
//
// 2014-01-16 : Sebastian Neubert
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltTrackReportsDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltTrackReportsDecoder::HltTrackReportsDecoder( const std::string& name,
                                                ISvcLocator* pSvcLocator )
: HltRawBankSplittingDecoder<LHCb::Tracks>( name , pSvcLocator,
                            KeyValue{ "RawEventLocations", Gaudi::Functional::concat_alternatives(LHCb::RawEventLocation::Trigger,
                                                                                                  LHCb::RawEventLocation::Copied,
                                                                                                  LHCb::RawEventLocation::Default )},
                            KeyValues{ "OutputHltTrackReportsLocation", prefix( "Hlt/Track", DefaultLabels ) }
                            )
{
    declareProperty( "OutputSourceId", m_map = { 1, 2, 4 } );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltTrackReportsDecoder::initialize()
{
    StatusCode sc = Decoder::AlgBase::initialize(); // must be executed first
    if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm

    // Check validity of source IDs: max is 7, must be unique...
    if ( std::any_of( std::begin(m_map), std::end(m_map), [](unsigned entry ) {
            return entry > HltTrackReportsWriter::kSourceID_Max;
    }) ) {
        return Error( "Illegal SourceID specified; maximal allowed value is 7",
                      StatusCode::FAILURE );
    }

    std::vector<unsigned> ids = m_map;
    std::sort(std::begin(ids), std::end(ids));
    if ( std::unique(std::begin(ids),std::end(ids)) != std::end(ids) ) {
        return Error( "Duplicate SourceID specified", StatusCode::FAILURE );
    }
    return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

Gaudi::Functional::vector_of_optional_<Tracks> HltTrackReportsDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
    ++m_callcount;

    // ----------------------------------------------------------
    // get the bank from RawEvent
    // ----------------------------------------------------------
    const auto& htr_banks = rawEvent.banks( RawBank::HltTrackReports );
    std::vector<const RawBank*> hltTrackReportsRawBanks = { htr_banks.begin(), htr_banks.end() };

    // check for bad banks:
    hltTrackReportsRawBanks.erase(
        std::remove_if( std::begin( hltTrackReportsRawBanks ),
                        std::end( hltTrackReportsRawBanks ), [=]( const RawBank* bank ) {
            if ( bank->magic() != RawBank::MagicPattern ) {
                Warning( " HltTrackReports RawBank has wrong magic number. Skipped ",
                         StatusCode::SUCCESS, 20 ).ignore();
                return true;
            }
            return false;
        } ),
        std::end( hltTrackReportsRawBanks ) );

    // sort by sourceID ( which is 'origin << somebits | sequential number' )
    std::sort( begin( hltTrackReportsRawBanks ), end( hltTrackReportsRawBanks ), 
               [=]( const RawBank* lhs, const RawBank* rhs ) 
               { return lhs->sourceID() < rhs->sourceID(); } );

    // TODO: add some counters to track how many tracks per source ID per event...
    auto indx = 0;
    Gaudi::Functional::vector_of_optional_<Tracks> outputs(m_map.size());
    for ( const auto & entry : m_map ) {
        auto& outputTracks = outputs[indx++];

        auto range = std::equal_range( begin(hltTrackReportsRawBanks), 
                                       end(hltTrackReportsRawBanks), entry, cmp_sourceID );
        // if there is a valid bank, create the output -- even if it is an empty bank...
        // (which results in an empty output ;-). If there is no bank, then do NOT
        // create the output...
        if ( range.first == range.second ) {
            //warning() << "Empty bank " << entry.first << endmsg;
            continue;
        }

        // map source id -> index of outputs

        outputTracks.emplace(); // create an empty container!

        // verify all present in expected order...
        bool ok = true;
        std::accumulate( range.first, range.second, 0, [&ok]( int i, const RawBank* b ) {
            ok = ok && i == seq( *b );
            return ++i;
        } );
        if ( !ok )
            throw GaudiException( "Missing Sequential HltTrackReports RawBank part - quiting.",
                                  name(), StatusCode::SUCCESS );

        // figure out the total size -- zero length (i.e. empty) bank is NOT an error
        unsigned int bankSize = std::accumulate( range.first,
                                                 range.second, 0u,
                                                 []( unsigned int s, const RawBank* bank ) {
            return s +
                   std::distance( bank->begin<unsigned int>(), bank->end<unsigned int>() );
        } );
        if ( std::distance( range.first, range.second)  == 1 ) {
            // avoid allocating and copying the rawbank if there is only a single one...
            // do the actual decoding: see HltTrackingCoder.cpp
            decodeTracks( (*range.first)->data(), bankSize, *outputTracks );
        } else {
            // concatenate banks into a local array
            std::vector<unsigned int> completeBank( bankSize );
            auto p = std::accumulate( range.first, range.second, completeBank.data(),
                             []( unsigned int* p, const RawBank* bank ) {
                return std::copy( bank->begin<unsigned int>(), bank->end<unsigned int>(), p );
            } );
            assert( std::distance( completeBank.data(), p ) == bankSize ); 
            // do the actual decoding: see HltTrackingCoder.cpp
            decodeTracks( completeBank.data(), bankSize, *outputTracks );
        }

        counter( std::to_string( entry )) += outputTracks->size();
        // for debug purposes print the contents of the outputLocation
        if ( msgLevel( MSG::VERBOSE ) ) {
            verbose() << "----------------------------------------\n";
            verbose() << "Decoded event " << m_callcount << endmsg;
            verbose() << "Decoding source id " << entry  <<endmsg;
            verbose() << outputTracks->size() << " Resurrected tracks: \n";
            std::for_each( std::begin(*outputTracks), std::end(*outputTracks),
                [&](const LHCb::Track* track) {
                verbose() << *track << endmsg;
                verbose() << "LHCbIDs: [\n";
                std::for_each( std::begin(track->lhcbIDs()), std::end(track->lhcbIDs()),
                               [&](const LHCb::LHCbID& id) {  verbose() << id << ",\n";} );
                verbose() << "]" << endmsg;
            } );
        }
    }
    return outputs;
}
#pragma GCC diagnostic pop
