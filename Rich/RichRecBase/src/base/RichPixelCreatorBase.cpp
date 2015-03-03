
//---------------------------------------------------------------------------------
/** @file RichPixelCreatorBase.cpp
 *
 *  Implementation file for tool base class : RichPixelCreatorBase
 *
 *  CVS Log :-
 *  $Id: RichPixelCreatorBase.cpp,v 1.13 2006-06-14 22:04:02 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   20/04/2005
 */
//---------------------------------------------------------------------------------

// local
#include "RichRecBase/RichPixelCreatorBase.h"

// namespaces
using namespace LHCb;

//-----------------------------------------------------------------------------

// Standard constructor, initializes variables
RichPixelCreatorBase::RichPixelCreatorBase( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
  : RichRecToolBase ( type, name, parent ),
    m_allDone       ( false ),
    m_richSys       ( 0 ),
    m_recGeom       ( 0 ),
    m_hpdOcc        ( 0 ),
    m_idTool        ( 0 ),
    m_decoder       ( 0 ),
    m_pixels        ( 0 ),
    m_bookKeep      ( false ),
    m_hpdCheck      ( false ),
    m_usedDets      ( Rich::NRiches, true ),
    m_richRecPixelLocation ( RichRecPixelLocation::Default ),
    m_begins        ( boost::extents[Rich::NRiches][Rich::NHPDPanelsPerRICH] ),
    m_ends          ( boost::extents[Rich::NRiches][Rich::NHPDPanelsPerRICH] ),
    m_Nevts         ( 0 ),
    m_hasBeenCalled ( false ),
    m_moniHPDOcc    ( true  )
{

  // Define the interface
  declareInterface<IRichPixelCreator>(this);

  if      ( context() == "Offline" )
  {
    m_richRecPixelLocation = RichRecPixelLocation::Offline;
  }
  else if ( context() == "HLT" )
  {
    m_richRecPixelLocation = RichRecPixelLocation::HLT;
  }

  // Define job option parameters
  declareProperty( "DoBookKeeping",       m_bookKeep  );
  declareProperty( "UseDetectors",        m_usedDets  );
  declareProperty( "CheckHPDsAreActive",  m_hpdCheck  );
  declareProperty( "ApplyPixelSuppression", m_moniHPDOcc );

}

StatusCode RichPixelCreatorBase::initialize()
{
  // base class initilize
  const StatusCode sc = RichRecToolBase::initialize();
  if ( sc.isFailure() ) { return sc; }

  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "RichRecPixel location : " << m_richRecPixelLocation << endreq;
  }

  // get tools
  acquireTool( "RichRecGeometry",    m_recGeom );
  //acquireTool( "RichSmartIDTool",    m_idTool,  0, true );
  //acquireTool( "RichSmartIDDecoder", m_decoder, 0, true );
  if ( m_hpdCheck )
  {
    m_richSys = getDet<DeRichSystem>( DeRichLocation::RichSystem );
    Warning( "Will check each pixel for HPD status. Takes additional CPU.",
             StatusCode::SUCCESS );
  }

  if ( m_moniHPDOcc ) acquireTool( "PixelSuppress", m_hpdOcc, this );

  // Check which detectors to use
  if ( !m_usedDets[Rich::Rich1] )
    Warning( "Pixels for RICH1 are disabled", StatusCode::SUCCESS );
  if ( !m_usedDets[Rich::Rich2] )
    Warning( "Pixels for RICH2 are disabled", StatusCode::SUCCESS );

  // Setup incident services
  incSvc()->addListener( this, IncidentType::BeginEvent );
  incSvc()->addListener( this, IncidentType::EndEvent   );

  // Intialise counts
  m_hitCount[Rich::Rich1] = 0;
  m_hitCount[Rich::Rich2] = 0;
  m_suppressedHitCount[Rich::Rich1] = 0;
  m_suppressedHitCount[Rich::Rich2] = 0;

  return sc;
}

StatusCode RichPixelCreatorBase::finalize()
{
  // print stats
  printStats();

  // base class finalize
  return RichRecToolBase::finalize();
}

void RichPixelCreatorBase::printStats() const
{
  if ( m_Nevts > 0
       && !( m_hitCount[Rich::Rich1] == 0 &&
             m_hitCount[Rich::Rich2] == 0 &&
             m_suppressedHitCount[Rich::Rich1] == 0 &&
             m_suppressedHitCount[Rich::Rich2] == 0
             ) )
  {
    const RichStatDivFunctor occ("%8.2f +-%7.2f");
    info() << "======================================================================================" << endreq
           << "                       Pixel Creator Summary for " << m_Nevts << " events :-" << endreq
           << "  Selected   :  RICH1 = " << occ(m_hitCount[Rich::Rich1],m_Nevts)
           << "  RICH2 = " << occ(m_hitCount[Rich::Rich2],m_Nevts)
           << "  pixels/event" << endreq
           << "  Rejected   :  RICH1 = " << occ(m_suppressedHitCount[Rich::Rich1],m_Nevts)
           << "  RICH2 = " << occ(m_suppressedHitCount[Rich::Rich2],m_Nevts)
           << "  pixels/event" << endreq
           << "======================================================================================" << endreq;
  }
  else
  {
    Warning( "No pixels produced", StatusCode::SUCCESS );
  }
}

RichRecPixel *
RichPixelCreatorBase::buildPixel( const RichSmartID id ) const
{

  // See if this RichRecPixel already exists
  RichRecPixel * pixel = ( bookKeep() && m_pixelDone[id] ? m_pixelExists[id] : 0 );
  if ( pixel ) return pixel;

  // Check this hit is OK
  if ( pixelIsOK(id) )
  {

    // Make a new RichRecPixel
    const Gaudi::XYZPoint gPos = smartIDTool()->globalPosition( id );
    pixel = new RichRecPixel( id,                              // SmartID for pixel
                              gPos,                            // position in global coords
                              smartIDTool()->globalToPDPanel(gPos), // position in local coords
                              Rich::PixelParent::RawBuffer,    // parent type
                              0                                // pointer to parent (not available)
                              );

    // compute corrected local coordinates
    computeRadCorrLocalPositions( pixel );

    // save to TES container in tool
    savePixel( pixel );

  }

  // Add to reference map
  if ( bookKeep() )
  {
    m_pixelExists[ id ] = pixel;
    m_pixelDone  [ id ] = true;
  }

  return pixel;
}

StatusCode RichPixelCreatorBase::newPixels() const
{
  if ( !m_allDone )
  {
    m_allDone = true; // only once per event

    // Obtain RichSmartIDs
    const RichDAQ::PDMap & smartIDs = smartIDdecoder()->allRichSmartIDs();

    // Reserve space
    richPixels()->reserve( smartIDs.size() );

    // Loop over HPDs and RichSmartIDs and create working pixels
    for ( RichDAQ::PDMap::const_iterator iHPD = smartIDs.begin();
          iHPD != smartIDs.end(); ++iHPD )
    {

      // apply HPD pixel suppression
      // NB : taking a copy of the smartIDs here since we might remove
      // some, and we cannot change the raw data from smartIDdecoder()
      LHCb::RichSmartID::Vector smartIDs = (*iHPD).second;
      applyPixelSuppression( (*iHPD).first, smartIDs );

      // create working pixels from suppressed smart IDs
      for ( RichSmartID::Vector::const_iterator iID = smartIDs.begin();
            iID != smartIDs.end(); ++iID )
      {
        // Make a Pixel for this RichSmartID
        buildPixel(*iID);
      }


    } // loop over HPDs

    // find iterators
    // note : we are relying on the sorting of the input RichSmartIDs here, so we
    // don't manually sort the RichRecPixels for speed unlike in other tool
    // implementations
    fillIterators();

    // Debug messages
    if ( msgLevel(MSG::DEBUG) )
    {
      debug() << "Created " << richPixels()->size() << " RichRecPixels at "
              << pixelLocation() << endreq;
    }

  }

  return StatusCode::SUCCESS;
}

void RichPixelCreatorBase::fillIterators() const
{

  // Reset to defaults
  resetIterators();

  // if there are some pixels, compute iterators
  // Should eventually look into if this can be done
  // whilst filling, avoiding the need for the "after-the -fact" loop

  if ( !richPixels()->empty() )
  {

    RichRecPixels::iterator iPix = richPixels()->begin();
    Rich::DetectorType rich      = (*iPix)->smartID().rich();
    Rich::Side        panel      = (*iPix)->smartID().panel();
    RichSmartID       hpd        = (*iPix)->smartID().hpdID();
    Rich::DetectorType lastrich  = rich;
    Rich::Side        lastpanel  = panel;
    RichSmartID       lasthpd    = hpd;
    ++iPix; // skip first pixel

    // loop over remaining pixels
    for ( ; iPix != richPixels()->end(); ++iPix )
    {
      if ( panel != (*iPix)->smartID().panel() )
      {
        panel = (*iPix)->smartID().panel();
        m_begins[(*iPix)->smartID().rich()][panel] = iPix;
        m_ends[lastrich][lastpanel]                = iPix;
        lastpanel = panel;
      }
      if ( rich != (*iPix)->smartID().rich() )
      {
        rich                = (*iPix)->smartID().rich();
        m_richBegin[rich]   = iPix;
        m_richEnd[lastrich] = iPix;
        lastrich            = rich;
      }
      if ( hpd != (*iPix)->smartID().hpdID() )
      {
        hpd                 = (*iPix)->smartID().hpdID();
        m_hpdBegin[hpd]     = iPix;
        m_hpdEnd[lasthpd]   = iPix;
        lasthpd             = hpd;
      }
    }

    // Set final iterators
    m_richEnd[rich]     = iPix;
    m_ends[rich][panel] = iPix;
    m_hpdEnd[hpd]       = iPix;

  }

}

RichRecPixels * RichPixelCreatorBase::richPixels() const
{

  // If first time for this event, either create or load the RichRecPixels
  if ( !m_pixels )
  {

    if ( !exist<RichRecPixels>(pixelLocation()) )
    {

      // Reinitialise the Pixel Container
      m_pixels = new RichRecPixels();

      // Register new RichRecPhoton container to Gaudi data store
      put( m_pixels, pixelLocation() );

    }
    else
    {

      // get pixels from TES
      m_pixels = get<RichRecPixels>( pixelLocation() );
      if ( msgLevel(MSG::DEBUG) )
      {
        debug() << "Found " << m_pixels->size() << " pre-existing RichRecPixels in TES at "
                << pixelLocation() << endreq;
      }

      if ( bookKeep() )
      {
        // Remake local pixel reference map
        for ( RichRecPixels::const_iterator iPixel = m_pixels->begin();
              iPixel != m_pixels->end(); ++iPixel )
        {
          m_pixelExists [ (*iPixel)->smartID() ] = *iPixel;
          m_pixelDone   [ (*iPixel)->smartID() ] = true;
        }
      }

      // find iterators
      fillIterators();

    }
  }

  return m_pixels;
}

RichRecPixels::iterator
RichPixelCreatorBase::begin( const Rich::DetectorType rich ) const
{
  return m_richBegin[rich];
}

RichRecPixels::iterator
RichPixelCreatorBase::end( const Rich::DetectorType rich ) const
{
  return m_richEnd[rich];
}

RichRecPixels::iterator
RichPixelCreatorBase::begin( const Rich::DetectorType rich,
                             const Rich::Side         panel ) const
{
  return m_begins[rich][panel];
}

RichRecPixels::iterator
RichPixelCreatorBase::end( const Rich::DetectorType rich,
                           const Rich::Side         panel ) const
{
  return m_ends[rich][panel];
}

RichRecPixels::iterator
RichPixelCreatorBase::begin( const LHCb::RichSmartID hpdID ) const
{
  HPDItMap::iterator i = m_hpdBegin.find(hpdID);
  // If not found, default to first pixel
  return ( i == m_hpdBegin.end() ? richPixels()->begin() : (*i).second );
}

RichRecPixels::iterator
RichPixelCreatorBase::end( const LHCb::RichSmartID hpdID ) const
{
  HPDItMap::iterator i = m_hpdEnd.find(hpdID);
  // If not found, default to first pixel
  return ( i == m_hpdEnd.end() ? richPixels()->begin() : (*i).second );
}

void RichPixelCreatorBase::handle ( const Incident& incident )
{
  // Update prior to start of event. Used to re-initialise data containers
  if ( IncidentType::BeginEvent == incident.type() )
  {
    this->InitNewEvent();
  }
  // Debug printout at the end of each event
  else if ( IncidentType::EndEvent == incident.type() )
  {
    this->FinishEvent();
  }
}

void RichPixelCreatorBase::InitNewEvent()
{
  m_hasBeenCalled = false;
  m_allDone = false;
  m_pixels  = 0;
  if ( m_bookKeep )
  {
    m_pixelExists.clear();
    m_pixelDone.clear();
  }
}

void RichPixelCreatorBase::FinishEvent()
{
  if ( m_hasBeenCalled ) ++m_Nevts;
  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Created " << richPixels()->size() << " RichRecPixels at "
            << pixelLocation() << endreq;
  }
}
