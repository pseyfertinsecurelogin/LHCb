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

#include <cmath>
#include <sstream>
#include <numeric>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/ToolHandle.h"

// from Gaudi
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/SystemOfUnits.h"

// Event
#include "Event/RawEvent.h"
#include "Event/OTLiteTime.h"

// OTDAQ
#include "OTDAQ/IOTRawBankDecoder.h"

// local
#include "OTDAQ/IndexedModuleDataHolder.h"


// forward declarations
namespace LHCb
{
  class RawBank ;
}

namespace LocalHelpers
{
  class DetectorHitData ;
}

class OTMultiBXRawBankDecoder : public extends<GaudiTool, IOTRawBankDecoder, IIncidentListener>
{

public:

  /// Standard constructor
  OTMultiBXRawBankDecoder( const std::string& type,
			   const std::string& name,
			   const IInterface* parent);

  /// Tool initialization
  StatusCode initialize() override;

  /// Tool finalize
  StatusCode finalize() override;

  /// Decode data for a single module
  LHCb::OTLiteTimeRange decodeModule( const LHCb::OTChannelID& moduleId ) const override;

  /// Decode all gol headers
  StatusCode decodeGolHeaders() const override { return m_decoder->decodeGolHeaders() ; }

  /// Decode all gol headers.
  StatusCode decodeGolHeaders(const LHCb::RawEvent& rawevent) const override { return m_decoder->decodeGolHeaders(rawevent) ;}

  /// Decode all modules
  StatusCode decode( LHCb::OTLiteTimeContainer& ottimes ) const override;

  /// Translate the raw bank in an ot-specific raw bank.
  StatusCode decode( OTDAQ::RawEvent& otevent ) const override;

  /// Retrieve the total number of hits in the OT without actually
  /// decoding the modules. Useful in pattern reco to remove full
  /// events.
  size_t totalNumberOfHits() const override;

  /// Get the conversion factor
  double nsPerTdcCount() const override { return m_decoder->nsPerTdcCount() ; }

  /// Create a single OTLiteTime
  LHCb::OTLiteTime time( LHCb::OTChannelID channel ) const override;

protected:
  void handle ( const Incident& incident ) override;
  StatusCode decodeAll() const ;

private:
  ToolHandle<IOTRawBankDecoder> m_decoder { "OTRawBankDecoder/OTSingleBXRawBankDecoder" };
  Gaudi::Property<std::vector<std::string>> m_rawEventLocations {
      this, "RawEventLocations", {LHCb::RawEventLocation::Default,
		                          "Prev1/DAQ/RawEvent",
		                          "Next1/DAQ/RawEvent"} };
  Gaudi::Property<bool> m_selectEarliestHit { this, "SelectEarliestHit",true} ;
  mutable std::unique_ptr<LocalHelpers::DetectorHitData> m_hitdata ;
};


namespace LocalHelpers
{

  struct ModuleHitData : public LHCb::OTLiteTimeContainer
  {
  public:
    LHCb::OTLiteTimeRange range() const { return LHCb::OTLiteTimeRange(begin(),end()) ; }
  } ;

  class DetectorHitData : public OTDAQ::IndexedModuleDataHolder<ModuleHitData>
  {
  public:
    DetectorHitData() : m_isDecoded(false) {}
    void clear() {
      m_isDecoded = false ;
      std::for_each( begin(), end(), [](LocalHelpers::ModuleHitData& mod) { mod.clear(); } );
    }
    size_t totalNumberOfHits() const {
      return std::accumulate( begin(), end(), size_t(0),
                              [](size_t n, const LocalHelpers::ModuleHitData& mod) {
                                  return n + mod.size();
      });
    }
    bool isDecoded() const { return m_isDecoded ; }
    void setIsDecoded(bool b=true) { m_isDecoded = b ; }

  private:
    bool m_isDecoded ;

  } ;

}

using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( OTMultiBXRawBankDecoder )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
OTMultiBXRawBankDecoder::OTMultiBXRawBankDecoder( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent )
: base_class ( type, name , parent )
{
  declareProperty("RawBankDecoder",m_decoder);
}
//=============================================================================

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode OTMultiBXRawBankDecoder::initialize()
{
  std::stringstream msg ;
  msg <<  "Merging raw events from following locations: " ;
  for( auto ilocation = m_rawEventLocations.begin() ;
       ilocation != m_rawEventLocations.end(); ++ilocation)
    msg << "\'" << *ilocation << "\', " ;
  info() << msg.str() << endmsg ;

  StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  sc = m_decoder.retrieve() ;
  if( sc.isFailure() ) {
    error() << "Failed to retieve decoder" << endmsg ;
    return sc ;
  } else {
    info() << "retrieved single BX decoder: " << m_decoder->type() << "/" << m_decoder->name() << endmsg ;
  }

  // Setup incident services
  incSvc()->addListener( this, IncidentType::BeginEvent );

  // initialize the decoder data. this things basically contains the decoded event
  m_hitdata.reset(new LocalHelpers::DetectorHitData());

  return sc ;
}

//=============================================================================
// Finalize
//=============================================================================
StatusCode OTMultiBXRawBankDecoder::finalize()
{
  m_hitdata.reset();
  m_decoder.release().ignore() ;
  return base_class::finalize() ;
}

//=============================================================================
// Handle for incident service
//=============================================================================

void OTMultiBXRawBankDecoder::handle ( const Incident& incident )
{
  if ( IncidentType::BeginEvent == incident.type() ) m_hitdata->clear() ;
}


size_t OTMultiBXRawBankDecoder::totalNumberOfHits() const
{
  if( !m_hitdata->isDecoded() ) decodeAll() ;
  return m_hitdata->totalNumberOfHits() ;
}

LHCb::OTLiteTimeRange OTMultiBXRawBankDecoder::decodeModule( const LHCb::OTChannelID& moduleid ) const
{
  if( !m_hitdata->isDecoded() ) decodeAll() ;
  return m_hitdata->module(moduleid.station(),moduleid.layer(),moduleid.quarter(),moduleid.module()).range() ;
}

StatusCode OTMultiBXRawBankDecoder::decode( LHCb::OTLiteTimeContainer& ottimes ) const
{
  if( !m_hitdata->isDecoded() ) decodeAll() ;
  auto numhits = m_hitdata->totalNumberOfHits() ;
  ottimes.clear() ;
  ottimes.reserve( numhits ) ;
  for( const auto& mod : *m_hitdata )
    ottimes.insert(ottimes.end(), mod.begin(), mod.end() ) ;
  return StatusCode::SUCCESS ;
}

StatusCode OTMultiBXRawBankDecoder::decodeAll() const
{
  StatusCode sc = StatusCode::SUCCESS ;
  for( auto ilocation = m_rawEventLocations.begin() ;
       ilocation != m_rawEventLocations.end() && sc.isSuccess(); ++ilocation) {
    // Get the raw event if it exists at this location. Issue a warning otherwise
    const LHCb::RawEvent* event = NULL;
    event = getIfExists<LHCb::RawEvent>(evtSvc(),*ilocation);
    if( NULL == event ) {
      Warning(std::string("No raw event buffer at location ") + *ilocation,
	      StatusCode::SUCCESS,0).ignore() ;
    } else {
      // from the name of the location, get the time offset
      double eventoffset(0) ;
      size_t pos(0) ;
      if(       (pos= ilocation->find("Prev") ) != std::string::npos) {
        int index = atoi( &((*ilocation)[pos+4]) ) ;
        eventoffset = index * -25*Gaudi::Units::ns ;
      } else if( (pos= ilocation->find("Next") ) != std::string::npos) {
        int index = atoi( &((*ilocation)[pos+4]) ) ;
        eventoffset = index * 25*Gaudi::Units::ns ;
      }
      debug() << *ilocation << " ----> time offset = " << eventoffset << endmsg ;

      // now set up the decoder for this event. without this call, it would just take the default event location.
      sc = m_decoder->decodeGolHeaders( *event ) ;
      if( sc.isSuccess() ) {
	// then get the hits
        LHCb::OTLiteTimeContainer ottimes ;
        sc = m_decoder->decode(ottimes) ;
        if( sc.isSuccess() ) {
          // we could optimize this by asking for the hits per module,
          // but that's not worth it right now.
          for( LHCb::OTLiteTimeContainer::const_iterator ihit = ottimes.begin() ;
               ihit != ottimes.end(); ++ihit ) {
            // create a new hit, adjusting for the offset
            LHCb::OTLiteTime newhit( ihit->channel(), eventoffset + ihit->calibratedTime() ) ;
            // find the module
            LocalHelpers::ModuleHitData& module = m_hitdata->module( ihit->channel() ) ;
            // check that this hit is not yet there. if 'select earliest
            // is set', we replace the previous hit if the new hit is
            // earlier. if not, we allow hits with different time to
            // co-exist.
            LHCb::OTLiteTimeContainer::iterator jhit = module.begin() ;
            if( m_selectEarliestHit ) {
              for( ; jhit != module.end() &&
                     newhit.channel().straw() != jhit->channel().straw() ; ++jhit) {}
              if( jhit != module.end() &&
                  jhit->calibratedTime() > newhit.calibratedTime() )
                *jhit = newhit ;
            } else {
              for( ; jhit != module.end() &&
                     !(newhit.channel().straw() == jhit->channel().straw() &&
                       std::abs( jhit->calibratedTime() - newhit.calibratedTime() )<1) ; ++jhit){}
            }
            if( jhit == module.end() )
              module.push_back( newhit ) ;
          }
        }
      }
    }
  }
  m_hitdata->setIsDecoded( true ) ;
  return sc ;
}

StatusCode OTMultiBXRawBankDecoder::decode( OTDAQ::RawEvent& otrawevent) const
{
  return m_decoder->decode( otrawevent ) ;
}

LHCb::OTLiteTime OTMultiBXRawBankDecoder::time( LHCb::OTChannelID channel ) const
{
  if( !m_hitdata->isDecoded() ) decodeAll() ;
  const LocalHelpers::ModuleHitData& moduledata = m_hitdata->module( channel );
  // if we would sort them, we could use a binary search ...
  auto jhit = std::find_if(moduledata.begin(), moduledata.end(),
                           [&](const LHCb::OTLiteTime& t) {
                  return t.channel().straw()==channel.straw();
  });
  if ( jhit == moduledata.end() ) {
    error() << "Cannot find channel in list of hits" << endmsg ;
    return LHCb::OTLiteTime{};
  }
  return *jhit;
}
