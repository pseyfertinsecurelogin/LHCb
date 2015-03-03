
//-----------------------------------------------------------------------------
/** @file RichTrackSelectorBase.cpp
 *
 *  Implementation file for RICH reconstruction tool : RichTrackSelectorBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   12/08/2006
 */
//-----------------------------------------------------------------------------

// GaudiKernel
#include "GaudiKernel/IJobOptionsSvc.h"

// local
#include "RichRecBase/RichTrackSelectorBase.h"

//-----------------------------------------------------------------------------

// RICH namespace
namespace Rich
{
  namespace Rec
  {

    //=============================================================================
    // Standard constructor, initializes variables
    //=============================================================================
    TrackSelectorBase::TrackSelectorBase( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
      : Rich::Rec::ToolBase ( type, name , parent )
    {
      // interface
      declareInterface<ITrackSelector>(this);

      // Default track types to select
      if ( context() == "HLT" || context() == "Hlt" )
      {
        m_minPCut = 2.0;
        m_trNames = boost::assign::list_of("Forward");
      }
      else // offline
      { 
        m_minPCut = 0.0;
        m_trNames =
          boost::assign::list_of("Forward")("Match")("Seed")("VeloTT")("KsTrack");
      }
      declareProperty( "TrackAlgs", m_trNames );

      declareProperty( "MinPCut",       m_minPCut     = 0.0 ); // in GeV
      declareProperty( "MaxPCut",       m_maxPCut     = 500 ); // in GeV

      declareProperty( "MinPtCut",      m_minPtCut    = 0.0 ); // in GeV
      declareProperty( "MaxPtCut",      m_maxPtCut    = 500 ); // in GeV

      declareProperty( "MinChi2Cut",    m_minChi2Cut  = 0.0 );
      declareProperty( "MaxChi2Cut",    m_maxChi2Cut  = boost::numeric::bounds<double>::highest() );

      declareProperty( "Charge",        m_chargeSel   = 0   );

      //declareProperty( "MinCloneDistCut", m_minCloneCut    = -1e10 );
      declareProperty( "MinCloneDistCut", m_minCloneCut    = boost::numeric::bounds<double>::lowest() );
      declareProperty( "MaxCloneDistCut", m_maxCloneCut    = boost::numeric::bounds<double>::highest() );

      declareProperty( "MinGhostProbCut", m_minGhostProb   = boost::numeric::bounds<double>::lowest() );
      declareProperty( "MaxGhostProbCut", m_maxGhostProb   = boost::numeric::bounds<double>::highest() );

      declareProperty( "AcceptClones", m_acceptClones = false );

      declareProperty( "RejectNonIsolated", m_rejectNonIsolated = false );

      //declareProperty( "MinLikelihood", m_minLL = -100 );
      declareProperty( "MinLikelihood", m_minLL = boost::numeric::bounds<double>::lowest() );
      declareProperty( "MaxLikelihood", m_maxLL = boost::numeric::bounds<double>::highest() );

      m_jobOpts =
        boost::assign::list_of
        ("MinPCut")("MaxPCut")("MinPtCut")("MaxPtCut")
        ("MinChi2Cut")("MaxChi2Cut")("Charge")("MinCloneDistCut")("MaxCloneDistCut")
        ("MinGhostProbCut")("MaxGhostProbCut")
        ("MinLikelihood")("MaxLikelihood")("AcceptClones")("RejectNonIsolated");

    }

    //=============================================================================
    // Destructor
    //=============================================================================
    TrackSelectorBase::~TrackSelectorBase() {}

    //=============================================================================
    // initialisation
    //=============================================================================
    StatusCode TrackSelectorBase::initialize()
    {
      // Sets up various tools and services
      StatusCode sc = Rich::Rec::ToolBase::initialize();
      if ( sc.isFailure() ) { return sc; }

      // Set up tracks
      sc = setUpTracks();

      return sc;
    }

    StatusCode TrackSelectorBase::setUpTracks()
    {
      StatusCode sc = StatusCode::SUCCESS;

      // define tools to use for each track type
      // should try and find a better way to do this
      m_tkToolNames[Rich::Rec::Track::Forward]     = "Rich::Rec::ForwardTrackSelector";
      m_tkToolNames[Rich::Rec::Track::Match]       = "Rich::Rec::MatchTrackSelector";
      m_tkToolNames[Rich::Rec::Track::VeloTT]      = "Rich::Rec::VeloTTTrackSelector";
      m_tkToolNames[Rich::Rec::Track::Seed]        = "Rich::Rec::SeedTrackSelector";
      m_tkToolNames[Rich::Rec::Track::KsTrack]     = "Rich::Rec::KsTrackSelector";
      m_tkToolNames[Rich::Rec::Track::Velo]        = "Rich::Rec::BaseTrackSelector";
      m_tkToolNames[Rich::Rec::Track::Trigger]     = "Rich::Rec::BaseTrackSelector";
      m_tkToolNames[Rich::Rec::Track::MCRichTrack] = "Rich::Rec::BaseTrackSelector";

      info() << "Track Selection : " << m_trNames << endmsg;
      for ( TrackNames::const_iterator iName = m_trNames.begin();
            iName != m_trNames.end(); ++iName )
      {
        if      ( *iName == "Forward"     ) { sc = setUpTrack( Rich::Rec::Track::Forward ); }
        else if ( *iName == "Match"       ) { sc = setUpTrack( Rich::Rec::Track::Match   ); }
        else if ( *iName == "KsTrack"     ) { sc = setUpTrack( Rich::Rec::Track::KsTrack ); }
        else if ( *iName == "VeloTT"      ) { sc = setUpTrack( Rich::Rec::Track::VeloTT  ); }
        else if ( *iName == "Seed"        ) { sc = setUpTrack( Rich::Rec::Track::Seed    ); }
        else if ( *iName == "Velo"        ) { sc = setUpTrack( Rich::Rec::Track::Velo    ); }
        else if ( *iName == "Trigger"     ) { sc = setUpTrack( Rich::Rec::Track::Trigger ); }
        else if ( *iName == "MCRichTrack" ) { sc = setUpTrack( Rich::Rec::Track::MCRichTrack ); }
        else
        {
          sc = Warning( "Unknown track algorithm type '"+(*iName)+"'" );
        }
        // check track was setup ok
        if ( sc.isFailure() ) return sc;
      }

      // clear map to save a little space
      m_tkToolNames.clear();

      return sc;
    }

    StatusCode TrackSelectorBase::setUpTrack( const Rich::Rec::Track::Type type )
    {
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << "Acquiring RichTrackSelector '" << m_tkToolNames[type]
                  << "' for type " << Rich::text(type) << endmsg;

      StatusCode sc = StatusCode::SUCCESS;

      if ( "" == m_tkToolNames[type] )
      {
        return Error( "Selection tool for track algorithm '"+Rich::text(type)+"' undefined" );
      }

      // Copy job options to track specific tool
      sc = sc && propagateJobOptions( name(), name()+"."+Rich::text(type), m_jobOpts );

      // finally, load the tool
      m_tkTools[type] =
        tool<IBaseTrackSelector>( m_tkToolNames[type], Rich::text(type), this );

      return sc;
    }

    //=============================================================================
    // finalisation
    //=============================================================================
    StatusCode TrackSelectorBase::finalize()
    {
      // Execute base class method
      return Rich::Rec::ToolBase::finalize();
    }

    const std::vector<std::string> & TrackSelectorBase::selectedTracks() const
    {
      return m_trNames;
    }

    // Test if the given Track is selected
    bool TrackSelectorBase::trackSelected( const LHCb::Track * track ) const
    {
      // is this type selected ?
      const Rich::Rec::Track::Type type = Rich::Rec::Track::type(track);
      TrackTools::const_iterator iT = m_tkTools.find( type );
      if ( msgLevel(MSG::VERBOSE) && iT == m_tkTools.end() )
      {
        verbose() << "Track algorithm type " << Rich::Rec::text(type)
                  << " not selected" << endmsg;
      }
      return ( iT != m_tkTools.end() ? iT->second->trackSelected(track) : false );
    }

    // Test it the given RichRecTrack is selected
    bool TrackSelectorBase::trackSelected( const LHCb::RichRecTrack * track ) const
    {
      // is this type selected ?
      const Rich::Rec::Track::Type type = track->trackID().trackType();
      TrackTools::const_iterator iT = m_tkTools.find( type );
      if ( msgLevel(MSG::VERBOSE) && iT == m_tkTools.end() )
      {
        verbose() << "Track algorithm type " << Rich::Rec::text(type)
                  << " not selected" << endmsg;
      }
      return ( iT != m_tkTools.end() ? iT->second->trackSelected(track) : false );
    }

    //=============================================================================


    double TrackSelectorBase::minPCut()    const { return m_minPCut; }
    double TrackSelectorBase::maxPCut()    const { return m_maxPCut; }
    double TrackSelectorBase::minPtCut()   const { return m_minPtCut; }
    double TrackSelectorBase::maxPtCut()   const { return m_maxPtCut; }
    double TrackSelectorBase::minChi2Cut() const { return m_minChi2Cut; }
    double TrackSelectorBase::maxChi2Cut() const { return m_maxChi2Cut; }
    int    TrackSelectorBase::chargeSel()  const { return m_chargeSel; }
    double TrackSelectorBase::minCloneDistCut()  const { return m_minCloneCut; }
    double TrackSelectorBase::maxCloneDistCut()  const { return m_maxCloneCut; }
    double TrackSelectorBase::minLikelihoodCut() const { return m_minLL; }
    double TrackSelectorBase::maxLikelihoodCut() const { return m_maxLL; }
    double TrackSelectorBase::minGhostProbCut()  const { return m_minGhostProb; }
    double TrackSelectorBase::maxGhostProbCut()  const { return m_maxGhostProb; }


    double TrackSelectorBase::minLikelihoodCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? minLikelihoodCut() : 
               max(minLikelihoodCut(),iT->second->minLikelihoodCut()) );
    }

    double TrackSelectorBase::maxLikelihoodCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? maxLikelihoodCut() :
               min(maxLikelihoodCut(),iT->second->maxLikelihoodCut()) );
    }

    double TrackSelectorBase::minPCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? minPCut() : max(minPCut(),iT->second->minPCut()) );
    }

    double TrackSelectorBase::maxPCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? maxPCut() : min(maxPCut(),iT->second->maxPCut()) );
    }

    double TrackSelectorBase::minPtCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? minPtCut() : max(minPtCut(),iT->second->minPtCut()) );
    }

    double TrackSelectorBase::maxPtCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? maxPtCut() : min(maxPtCut(),iT->second->maxPtCut()) );
    }

    double TrackSelectorBase::minChi2Cut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? minChi2Cut() : max(minChi2Cut(),iT->second->minChi2Cut()) );
    }

    double TrackSelectorBase::maxChi2Cut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? maxChi2Cut() : min(maxChi2Cut(),iT->second->maxChi2Cut()) );
    }

    int TrackSelectorBase::chargeSel( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? chargeSel() : iT->second->chargeSel() );
    }

    double TrackSelectorBase::minCloneDistCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? 
               minCloneDistCut() : max(minCloneDistCut(),iT->second->minCloneDistCut()) );
    }

    double TrackSelectorBase::maxCloneDistCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? 
               maxCloneDistCut() : min(maxCloneDistCut(),iT->second->maxCloneDistCut()) );
    }

    double TrackSelectorBase::minGhostProbCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? 
               minGhostProbCut() : max(minGhostProbCut(),iT->second->minGhostProbCut()) );
    }

    double TrackSelectorBase::maxGhostProbCut( const Rich::Rec::Track::Type type ) const
    {
      TrackTools::const_iterator iT = m_tkTools.find(type);
      return ( iT == m_tkTools.end() ? 
               maxGhostProbCut() : min(maxGhostProbCut(),iT->second->maxGhostProbCut()) );
    }

  }
}
