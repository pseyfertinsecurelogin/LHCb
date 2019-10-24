/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/ITrackFitResult.h"
#include "Event/State.h"
#include "Event/TrackParameters.h"
#include "Event/TrackTags.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Range.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/HeaderMapping.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

/** @namespace v1
 *
 * Track is the base class for offline and online tracks.
 *
 * @author Jose Hernando, Eduardo Rodrigues
 *
 */

namespace LHCb::Event {
  // Forward declarations
  inline namespace v1 {
    // Forward declarations2

    // Class ID definition
    static const CLID CLID_Track = 10010;

    // Namespace for locations in TDS
    namespace TrackLocation {
      inline const std::string Default             = "Rec/Track/Best";
      inline const std::string Ideal               = "MC/Track/Ideal";
      inline const std::string Seed                = "Rec/Track/Seed";
      inline const std::string RZVelo              = "Rec/Track/RZVelo";
      inline const std::string Velo                = "Rec/Track/Velo";
      inline const std::string Forward             = "Rec/Track/Forward";
      inline const std::string VeloTT              = "Rec/Track/VeloTT";
      inline const std::string TT                  = "Rec/Track/TT";
      inline const std::string Match               = "Rec/Track/Match";
      inline const std::string Downstream          = "Rec/Track/Downstream";
      inline const std::string Tsa                 = "Rec/Track/Tsa";
      inline const std::string Muon                = "Rec/Track/Muon";
      inline const std::string VP                  = "Rec/Track/VP";
      inline const std::string VPTT                = "Rec/Track/VPTT";
      inline const std::string VeloUT              = "Rec/Track/VeloUT";
      inline const std::string UT                  = "Rec/Track/UT";
      inline const std::string CaloCosmicsForward  = "Calo/Track/Forward";
      inline const std::string CaloCosmicsBackward = "Calo/Track/Backward";
      inline const std::string HltImport           = "Red/Track/HLTImport";
    } // namespace TrackLocation

    /** @class Track Track.h
     *
     * Track is the base class for offline and online tracks.
     *
     * @author Jose Hernando, Eduardo Rodrigues
     *
     */

    class Track final : public KeyedObject<int> {
    public:
      /// typedef for std::vector of Track
      using Vector      = std::vector<Track*>;
      using ConstVector = std::vector<const Track*>;

      /// typedef for KeyedContainer of Track
      typedef KeyedContainer<Track, Containers::HashMap> Container;

      /// For defining SharedObjectContainer
      using Selection = SharedObjectsContainer<Track>;
      /// For accessing a list of tracks which is either a SharedObjectContainer, a KeyedContainer or a ConstVector
      using Range = Gaudi::Range_<ConstVector>;
      /// Vector of additional information
      typedef GaudiUtils::VectorMap<int, double> ExtraInfo;
      /// Container for LHCbIDs on track
      using LHCbIDContainer = std::vector<LHCbID>;
      /// Container for States on track
      using StateContainer = std::vector<State*>;

      /// Track history enumerations: each entry corresponds to the name of the pattern recognition algorithm that
      /// produced the Track
      enum History {
        HistoryUnknown   = 0,  // unknown history (i.e. history not set)
        TrackIdealPR     = 1,  // track produced with the ideal pattern recognition
        TrackSeeding     = 2,  // track produced with the seeding pattern recognition
        PatVelo          = 3,  // track produced with the 2D then 3D VELO pattern recognition
        PatVeloTT        = 4,  // track produced with the online Velo-TT pattern recognition
        TrackVeloTT      = 5,  // track produced with the offline Velo-TT pattern recognition
        PatForward       = 6,  // track produced with the forward pattern recognition
        TrackMatching    = 7,  //
        PatKShort        = 8,  // track produced with the PatKShort pattern recognition
        TsaTrack         = 9,  //
        MuonID           = 10, // track produced with the Muon pattern recognition
        PatVeloR         = 11, // produced with the 2D VELO pattern recognition
        PatVeloGeneric   = 12, // produced with the VELO Generic pattern recognition
        PatVeloGeneral   = 13, // produced with the VELO General pattern recognition
        PatVeloOpen      = 14, // produced with the VELO open pattern recognition
        PatSeeding       = 15, // track produced with the PatSeeding pattern recognition
        PatMatch         = 16, // track produced with the PatMatch pattern recognition
        PatDownstream    = 17, // track produced with the PatDownstream pattern recognition
        TTGeneric        = 18, // track produced with the TTGenericTracking algorithm
        PatVPCheated     = 19, // track produced with the CheatedVPPat algorithm
        HLTImportedTrack = 20, // track imported from HLT to offline
        PatFastVelo      = 21, // track produced by FastVelo algorithm
        PatVP            = 22, // track produced by VP algorithm
        PatVeloUT        = 23, // track produced with the online Velo-UT pattern recognition
        PrForward        = 30, // track produced with the PrForward pattern recognition for the upgrade
        PrSeeding        = 31, // track produced with the PrSeeding pattern recognition for the upgrade
        PrMatch          = 32, // track produced with the PrMatch pattern recognition for the upgrade
        PrDownstream     = 33, // track produced with the PrDownstream pattern recognition for the upgrade
        PrVeloUT         = 34  // track produced with the PrVeloUT pattern recognition for the upgrade
      };
      /// Track fit history enumerations
      enum FitHistory {
        FitUnknown = 0, // track not fitted yet (fit history not set)
        StdKalman,      // track fitted with the standard Kalman fitter
        BiKalman        // track fitted with the bi-directional Kalman fitter
      };
      /// Track type enumerations
      enum Types {
        TypeUnknown = 0, // track of undefined type
        Velo,            // VELO track
        VeloR,           // 2D VELO track
        Long,            // forward track
        Upstream,        // upstream track
        Downstream,      // downstream track
        Ttrack,          // seed track
        Muon,            // muon track
        Calo,            // calo cosmics track
        TT,              // TT track
        UT               // UT track
      };
      /// Track pattern recognition status flag enumerations: The flag specifies in which state of the pattern
      /// recognition phase the track is. The status flag is set by the relevant algorithms
      enum PatRecStatus {
        PatRecStatusUnknown = 0, // track in an undefined PR status
        PatRecIDs,               // pattern recognition track with LHCbIDs
        PatRecMeas               // pattern recognition track with Measurements added
      };
      /// Track fitting status flag enumerations: The flag specifies in which state of the fitting phase the track is.
      /// The status flag is set by the relevant algorithms
      enum FitStatus {
        FitStatusUnknown = 0, // track in an undefined fitting status
        Fitted,               // fitted track
        FitFailed             // track for which the track fit failed
      };
      /// Track general flags enumerations
      enum Flags {
        FlagsUnknown = 0,  //
        Backward     = 1,  // VELO track in the backward direction
        Invalid      = 2,  // invalid track for physics
        Clone        = 4,  // clone track (of a corresponding unique track)
        Used         = 8,  //
        IPSelected   = 16, //
        PIDSelected  = 32, //
        Selected     = 64, //
        L0Candidate  = 128 //
      };
      /// Additional information assigned to this Track by pattern recognition
      enum AdditionalInfo {
        AdditionalInfoUnknown = 0, //
        DC06Likelihood = 1, // The Likelihood the track is real. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        PatQuality     = 2, // Quality variable from PatForward Tracking
        Cand1stQPat    = 3, //  Quality of the first candidate
        Cand2ndQPat    = 4, //  Quality of the second candidate
        NCandCommonHits   = 5,  //  NCand with common hits
        Cand1stChi2Mat    = 6,  //  Chi2 of the first candidate
        Cand2ndChi2Mat    = 7,  //  Chi2 of the second candidate
        DC06nExpectedVelo = 10, // number of expected Velo hits. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        DC06nExpectedTT   = 11, // number of expected TT hits. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        DC06nExpectedIT   = 12, // number of expected IT hits. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        DC06nExpectedOT   = 13, // number of expected OT hits. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        MatchChi2         = 16, // Chi2 from the velo-seed matching (TrackMatching)
        FitVeloChi2       = 17, // Chi2 of the velo segment (from TrackFitResult)
        FitVeloNDoF       = 18, // NDoF of the velo segment chisq
        FitTChi2          = 19, // Chi2 of the T station segment (from TrackFitResult)
        FitTNDoF          = 20, // NDoF of the T station segment chisq
        FitMatchChi2      = 21, // Chi2 of the breakpoint between T and TT (from TrackFitResult)
        FitFracUsedOTTimes = 25, // Fraction of OT hits for which drifttime is used in fit
        TsaLikelihood      = 32, // Likelihood from tsa seeding
        CloneDist =
            101, // Track is flagged as being a (rejected) clone of another track. Value is the KL clone distance
        DC06GhostProbability =
            102, //  gives the NN ghost probability. OBSOLETE, may exist on DC06 DSTs and some 2007/09 files
        nPRVeloRZExpect    = 103, // Number of expected Velo clusters from VELO RZ pattern recognition
        nPRVelo3DExpect    = 104, // Number of expected Velo clusters from VELO 3D pattern recognition
        AdditionalInfo201  = 201, // OBSOLETE, may exist in some 2008/09 files
        AdditionalInfo202  = 202, // OBSOLETE, may exist in some 2008/09 files
        MuonChi2perDoF     = 300, // Chi2/nDoF of muon track fit
        MuonMomentumPreSel = 301, // 1 if pass Momentum pre-selection, 0 if not
        MuonInAcceptance   = 302, // 1 if in Muon system InAcceptance, 0 if not
        IsMuonLoose        = 303, // 1 if pass IsMuonLoose criteria, 0 if not
        IsMuon             = 304, // 1 if pass IsMuon criteria, 0 if not
        MuonDist2          = 305, // Squared distance of the closest muon hit to the extrapolated track
        MuonDLL            = 306, // DLL (from muon system only)
        MuonNShared = 307, // NShared (number of additional IsMuon tracks with at least one shared hit with the current
                           // track and a smaller Dist value)
        MuonCLQuality = 308, // CLQuality
        MuonCLArrival = 309, // CLArrival
        IsMuonTight   = 310  // 1 if pass IsMuonTight criteria, 0 if not
      };

      /// Constructor with assigned key
      explicit Track( int key )
          : KeyedObject<int>( key )
          , m_chi2PerDoF( 0.0 )
          , m_nDoF( 0 )
          , m_likelihood( 999 )
          , m_ghostProbability( 999 )
          , m_flags( 0 ) {}

      /// Move constructor
      Track( Track&& track )
          : KeyedObject<int>()
          , m_chi2PerDoF( track.m_chi2PerDoF )
          , m_nDoF( track.m_nDoF )
          , m_likelihood( track.m_likelihood )
          , m_ghostProbability( track.m_ghostProbability )
          , m_flags( track.m_flags )
          , m_lhcbIDs( std::move( track.m_lhcbIDs ) )
          , m_states( std::move( track.m_states ) )
          , m_fitResult( std::exchange( track.m_fitResult, nullptr ) )
          , m_extraInfo{std::move( track.m_extraInfo )}
          , m_ancestors( std::move( track.m_ancestors ) ) {}

      /// Move constructor
      Track( Track&& track, const int key )
          : KeyedObject<int>( key )
          , m_chi2PerDoF( track.m_chi2PerDoF )
          , m_nDoF( track.m_nDoF )
          , m_likelihood( track.m_likelihood )
          , m_ghostProbability( track.m_ghostProbability )
          , m_flags( track.m_flags )
          , m_lhcbIDs( std::move( track.m_lhcbIDs ) )
          , m_states( std::move( track.m_states ) )
          , m_fitResult( std::exchange( track.m_fitResult, nullptr ) )
          , m_extraInfo{std::move( track.m_extraInfo )}
          , m_ancestors( std::move( track.m_ancestors ) ) {}

      /// Constructor
      explicit Track( const History& history, const Types& trackType, const PatRecStatus& patStatus,
                      const std::vector<LHCbID>& ids, const State& aState );

      /// Constructor
      explicit Track( const History& history, const Types& trackType, const PatRecStatus& patStatus );

      /// Copy constructor
      Track( const Track& track );

      /// Copy constructor
      Track( const Track& track, const int key );

      /// Default Constructor
      Track() : m_lhcbIDs(), m_states(), m_fitResult(), m_extraInfo() {}

      /// Track destructor
      ~Track() override;

      // Retrieve pointer to class definition structure
      const CLID&        clID() const override;
      static const CLID& classID();

      /// conversion of string to enum for type History
      static LHCb::Event::v1::Track::History HistoryToType( const std::string& aName );

      /// conversion to string for enum type History
      static const std::string& HistoryToString( int aEnum );
      /// conversion of string to enum for type FitHistory
      static LHCb::Event::v1::Track::FitHistory FitHistoryToType( const std::string& aName );

      /// conversion to string for enum type FitHistory
      static const std::string& FitHistoryToString( int aEnum );
      /// conversion of string to enum for type Types
      static LHCb::Event::v1::Track::Types TypesToType( const std::string& aName );

      /// conversion to string for enum type Types
      static const std::string& TypesToString( int aEnum );
      /// conversion of string to enum for type PatRecStatus
      static LHCb::Event::v1::Track::PatRecStatus PatRecStatusToType( const std::string& aName );

      /// conversion to string for enum type PatRecStatus
      static const std::string& PatRecStatusToString( int aEnum );
      /// conversion of string to enum for type FitStatus
      static LHCb::Event::v1::Track::FitStatus FitStatusToType( const std::string& aName );

      /// conversion to string for enum type FitStatus
      static const std::string& FitStatusToString( int aEnum );
      /// conversion of string to enum for type Flags
      static LHCb::Event::v1::Track::Flags FlagsToType( const std::string& aName );

      /// conversion to string for enum type Flags
      static const std::string& FlagsToString( int aEnum );
      /// conversion of string to enum for type AdditionalInfo
      static LHCb::Event::v1::Track::AdditionalInfo AdditionalInfoToType( const std::string& aName );

      /// conversion to string for enum type AdditionalInfo
      static const std::string& AdditionalInfoToString( int aEnum );

      /// Move assignement operator
      Track& operator=( Track&& track );

      /// Retrieve the position and momentum vectors and the corresponding 6D covariance matrix (pos:0->2,mom:3-5) at
      /// the first state
      void positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom, Gaudi::SymMatrix6x6& cov6D ) const;

      /// Retrieve the position and momentum vectors at the first state
      void positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const;

      /// Retrieve the 3D-position vector at the first state
      Gaudi::XYZPoint position() const;

      /// Retrieve the 3D-position (+ errors) at the first state
      void position( Gaudi::XYZPoint& pos, Gaudi::SymMatrix3x3& errPos ) const;

      /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) and errors at the first state
      Gaudi::XYZVector slopes() const;

      /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) at the first state
      void slopes( Gaudi::XYZVector& slopes, Gaudi::SymMatrix3x3& errSlopes ) const;

      /// Retrieve the momentum vector at the first state
      Gaudi::XYZVector momentum() const;

      /// Retrieve the momentum at the first state
      double p() const;

      /// Retrieve the transverse momentum at the first state
      double pt() const;

      /// Retrieve the pseudorapidity at the first state
      double pseudoRapidity() const;

      /// Retrieve the phi at the first state
      double phi() const;

      /// Retrieve the momentum vector (and errors) at the first state
      void momentum( Gaudi::XYZVector& mom, Gaudi::SymMatrix3x3& errMom ) const;

      /// Retrieve the 6D (x,y,z,px,py,pz) covariance matrix at the first state
      void posMomCovariance( Gaudi::SymMatrix6x6& cov6D ) const;

      /// Retrieve the first state on the track
      State& firstState();

      /// Retrieve the first state on the track
      const State& firstState() const;

      /// Retrieve the charge assigned to the track
      int charge() const;

      /// Retrieve the Chi^2 of the track (fit)
      double chi2() const;

      /// Set the Chi^2 and the DoF of the track (fit)
      void setChi2AndDoF( double chi2, int ndof );

      /// Probability of chi2^2 of the track
      double probChi2() const;

      /// Retrieve the number of states on the track
      unsigned int nStates() const;

      /// Add a State to the list of States associated to the track
      void addToStates( const State& state );

      /// Add a set of states to the track. Track takes ownership. (note: const refers to the pointer, not the State!)
      void addToStates( span<State* const> states, Tag::State::AssumeUnordered_tag = {} );

      /// Add a set of sorted states by increasing Z to the track. Track takes ownership. (note: const refers to the
      /// pointer, not the State!)
      void addToStates( span<State* const> states, Tag::State::AssumeSorted_tag );

      /// Remove a State from the list of states associated to the track
      void removeFromStates( State* value );

      /// Clear the State vector
      void clearStates();

      /// Retrieve the reference to the state closest to the given z-position
      State& closestState( double z );

      /// Retrieve the reference to the state closest to the given z-position
      const State& closestState( double z ) const;

      /// Retrieve the reference to the state closest to the given plane
      const State& closestState( const Gaudi::Plane3D& plane ) const;

      /// Check the existence of a state at a certain predefined location (see the Location enum in State.h)
      bool hasStateAt( const State::Location& location ) const;

      /// Retrieve the state at a certain predefined location (see the Location enum in State.h)
      State* stateAt( const State::Location& location );

      /// Retrieve the const state at a certain predefined location (see the Location enum in State.h)
      const State* stateAt( const State::Location& location ) const;

      /// Retrieve the number of LHCbIDs on the track
      unsigned int nLHCbIDs() const;

      /// Add an LHCbID to the list of LHCbIDs associated to the track. Return true if LHCbID was not yet on track.
      bool addToLhcbIDs( const LHCbID& value );

      /// Add a sorted (!) sequence of LHCbID to the list of LHCbIDs associated to the track. Returns true if none of
      /// the LHCb ids was on the track.
      bool addSortedToLhcbIDs( span<const LHCbID> ids );

      /// Add an unsorted sequence of LHCbID to the list of LHCbIDs associated to the track. The vector will first be
      /// sorted, which makes this more expensive than the method above. Returns true if none of the LHCb ids was on the
      /// track.
      bool addToLhcbIDs( span<const LHCbID> ids );

      /// Sets the list of LHCbIDs associated to this track. The input vector will be sorted.
      void setLhcbIDs( span<const LHCbID> ids );

      /// Sets the list of LHCbIDs associated to this track. The input vector must be sorted.
      void setSortedLhcbIDs( span<const LHCbID> ids );

      /// Sets the list of LHCbIDs associated to this track. The input vector must be sorted.
      void setSortedLhcbIDs( LHCbIDContainer&& value );

      /// Returns true if the LHCbIDs of track are a subset is the LHCbIDs of this track.
      bool containsLhcbIDs( const Track& track ) const;

      /// Returns true if LHCbIDs in ids are a subset of the LHCbIDs of this track.
      bool containsLhcbIDs( span<const LHCbID> ids ) const;

      /// Returns the number of common LHCbIDs.
      size_t nCommonLhcbIDs( const Track& track ) const;

      /// Remove an LHCbID from the list of LHCbIDs associated to the track
      void removeFromLhcbIDs( const LHCbID& value );

      /// Add a track to the list of ancestors of this track
      void addToAncestors( const Track& ancestor );

      /// Set pointer to object holding track fit data. Track becomes owner.
      void setFitResult( ITrackFitResult* trackfit );

      /// get pointer to the object holding the trackfit data.
      ITrackFitResult* fitResult();

      /// get const pointer to the object holding the trackfit data.
      const ITrackFitResult* fitResult() const;

      /// Clear the track before re-use
      void reset();

      /// Copy the info from the argument track into this track
      void copy( const Track& track );

      /// Check the type of the track (see the Types enum)
      bool checkType( const Types& value ) const;

      /// Check the history of the track (see the History enum)
      bool checkHistory( const History& value ) const;

      /// Check the fit history of the track (see the FitHistory enum)
      bool checkFitHistory( const FitHistory& value ) const;

      /// Check the pattern recognition status of the track (see the PatRecStatus enum)
      bool checkPatRecStatus( const PatRecStatus& value ) const;

      /// Check the fitting status of the track (see the FitStatus enum)
      bool checkFitStatus( const FitStatus& value ) const;

      /// Update the flag (see the Flags enum)
      void setFlag( unsigned int flag, bool ok );

      /// Check the status of the flag (see the Flags enum)
      bool checkFlag( const Flags& flag ) const;

      /// Check if track is of a type that goes thro T stations
      bool hasT() const;

      /// Check if track is of a type that goes thro Velo
      bool hasVelo() const;

      /// Check if track is of a type that goes thro TT
      bool hasTT() const;

      /// Check if track is of a type that goes thro UT
      bool hasUT() const;

      /// Check whether the given LHCbID is on the track
      bool isOnTrack( const LHCbID& value ) const;

      /// printOut method to Gaudi message stream
      std::ostream& fillStream( std::ostream& os ) const override;

      /// Check whether the track has information for the specified key
      bool hasInfo( const int key ) const;

      /// Add new information, associated with the specified key. This method cannot be used to modify information for
      /// an already existing key
      bool addInfo( const int key, const double info );

      /// Extract the information associated with the specified key. If there is no such information the default value
      /// will be returned.
      double info( const int key, const double def ) const;

      /// Erase the information associated with the specified key
      Track::ExtraInfo::size_type eraseInfo( const int key );

      /// Retrieve const  Chi^2 per degree of freedom of the track
      double chi2PerDoF() const;

      /// Update  Chi^2 per degree of freedom of the track
      void setChi2PerDoF( double value );

      /// Retrieve const  Number of degrees of freedom of the track
      int nDoF() const;

      /// Update  Number of degrees of freedom of the track
      void setNDoF( int value );

      /// Retrieve const  Likelihood variable
      double likelihood() const;

      /// Update  Likelihood variable
      void setLikelihood( double value );

      /// Retrieve const  ghost probability variable
      double ghostProbability() const;

      /// Update  ghost probability variable
      void setGhostProbability( double value );

      /// Retrieve const  The variety of track flags
      unsigned int flags() const;

      /// Update  The variety of track flags
      void setFlags( unsigned int value );

      /// Retrieve Track type
      Types type() const;

      /// Update Track type
      void setType( const Types& value );

      /// Retrieve Specifies the pattern recognition algorithm that created the track
      History history() const;

      /// Update Specifies the pattern recognition algorithm that created the track
      void setHistory( const History& value );

      /// Retrieve Specifies the fitting algorithm the fitted the track)
      FitHistory fitHistory() const;

      /// Update Specifies the fitting algorithm the fitted the track)
      void setFitHistory( const FitHistory& value );

      /// Retrieve Pattern recognition status of the track
      PatRecStatus patRecStatus() const;

      /// Update Pattern recognition status of the track
      void setPatRecStatus( const PatRecStatus& value );

      /// Retrieve Fitting status of the track
      FitStatus fitStatus() const;

      /// Update Fitting status of the track
      void setFitStatus( const FitStatus& value );

      /// Retrieve Track flags
      Flags flag() const;

      /// Retrieve Track specific bits
      unsigned int specific() const;

      /// Update Track specific bits
      void setSpecific( unsigned int value );

      /// Retrieve const  Container of (sorted) LHCbIDs
      const std::vector<LHCbID>& lhcbIDs() const;

      /// Retrieve const  Container with pointers to all the states
      const std::vector<State*>& states() const;

      /// Retrieve  Container with pointers to all the states
      std::vector<State*>& states();

      /// Retrieve const  Additional pattern recognition information. Do not access directly, use *Info() methods
      /// instead.
      const ExtraInfo& extraInfo() const;

      /// Update  Additional pattern recognition information. Do not access directly, use *Info() methods instead.
      void setExtraInfo( const ExtraInfo& value );

      /// Retrieve (const)  Ancestor tracks that created this one
      const SmartRefVector<Track>& ancestors() const;

      /// Retrieve  Ancestor tracks that created this one
      SmartRefVector<Track>& ancestors();

      /// Add to  Ancestor tracks that created this one
      void addToAncestors( const SmartRef<Track>& value );

      /// Att to (pointer)  Ancestor tracks that created this one
      void addToAncestors( const Track* value );

      /// Remove from  Ancestor tracks that created this one
      void removeFromAncestors( const SmartRef<Track>& value );

      /// Clear  Ancestor tracks that created this one
      void clearAncestors();

      friend std::ostream& operator<<( std::ostream& str, const Track& obj ) { return obj.fillStream( str ); }

    protected:
    private:
      /// Offsets of bitfield flags
      enum flagsBits {
        typeBits         = 0,
        historyBits      = 4,
        fitHistoryBits   = 11,
        patRecStatusBits = 14,
        fitStatusBits    = 16,
        flagBits         = 18,
        specificBits     = 28
      };

      /// Bitmasks for bitfield flags
      enum flagsMasks {
        typeMask         = 0xfL,
        historyMask      = 0x7f0L,
        fitHistoryMask   = 0x3800L,
        patRecStatusMask = 0xc000L,
        fitStatusMask    = 0x30000L,
        flagMask         = 0xffc0000L,
        specificMask     = 0xf0000000L
      };

      double                           m_chi2PerDoF{0.0};       ///< Chi^2 per degree of freedom of the track
      int                              m_nDoF{0};               ///< Number of degrees of freedom of the track
      double                           m_likelihood{999};       ///< Likelihood variable
      double                           m_ghostProbability{999}; ///< ghost probability variable
      unsigned int                     m_flags{0};              ///< The variety of track flags
      std::vector<LHCbID>              m_lhcbIDs;               ///< Container of (sorted) LHCbIDs
      std::vector<State*>              m_states;                ///< Container with pointers to all the states
      std::unique_ptr<ITrackFitResult> m_fitResult; ///< Transient data related to track fit (nodes, material, etc)
      ExtraInfo m_extraInfo; ///< Additional pattern recognition information. Do not access directly, use *Info()
                             ///< methods instead.
      SmartRefVector<Track> m_ancestors; ///< Ancestor tracks that created this one

      static const GaudiUtils::VectorMap<std::string, History>&        s_HistoryTypMap();
      static const GaudiUtils::VectorMap<std::string, FitHistory>&     s_FitHistoryTypMap();
      static const GaudiUtils::VectorMap<std::string, Types>&          s_TypesTypMap();
      static const GaudiUtils::VectorMap<std::string, PatRecStatus>&   s_PatRecStatusTypMap();
      static const GaudiUtils::VectorMap<std::string, FitStatus>&      s_FitStatusTypMap();
      static const GaudiUtils::VectorMap<std::string, Flags>&          s_FlagsTypMap();
      static const GaudiUtils::VectorMap<std::string, AdditionalInfo>& s_AdditionalInfoTypMap();

    }; // class Track

    /// Definition of Keyed Container for Track
    typedef KeyedContainer<Track, Containers::HashMap> Tracks;

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::History e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::HistoryUnknown:
        return s << "HistoryUnknown";
      case LHCb::Event::v1::Track::TrackIdealPR:
        return s << "TrackIdealPR";
      case LHCb::Event::v1::Track::TrackSeeding:
        return s << "TrackSeeding";
      case LHCb::Event::v1::Track::PatVelo:
        return s << "PatVelo";
      case LHCb::Event::v1::Track::PatVeloTT:
        return s << "PatVeloTT";
      case LHCb::Event::v1::Track::TrackVeloTT:
        return s << "TrackVeloTT";
      case LHCb::Event::v1::Track::PatForward:
        return s << "PatForward";
      case LHCb::Event::v1::Track::TrackMatching:
        return s << "TrackMatching";
      case LHCb::Event::v1::Track::PatKShort:
        return s << "PatKShort";
      case LHCb::Event::v1::Track::TsaTrack:
        return s << "TsaTrack";
      case LHCb::Event::v1::Track::MuonID:
        return s << "MuonID";
      case LHCb::Event::v1::Track::PatVeloR:
        return s << "PatVeloR";
      case LHCb::Event::v1::Track::PatVeloGeneric:
        return s << "PatVeloGeneric";
      case LHCb::Event::v1::Track::PatVeloGeneral:
        return s << "PatVeloGeneral";
      case LHCb::Event::v1::Track::PatVeloOpen:
        return s << "PatVeloOpen";
      case LHCb::Event::v1::Track::PatSeeding:
        return s << "PatSeeding";
      case LHCb::Event::v1::Track::PatMatch:
        return s << "PatMatch";
      case LHCb::Event::v1::Track::PatDownstream:
        return s << "PatDownstream";
      case LHCb::Event::v1::Track::TTGeneric:
        return s << "TTGeneric";
      case LHCb::Event::v1::Track::PatVPCheated:
        return s << "PatVPCheated";
      case LHCb::Event::v1::Track::HLTImportedTrack:
        return s << "HLTImportedTrack";
      case LHCb::Event::v1::Track::PatFastVelo:
        return s << "PatFastVelo";
      case LHCb::Event::v1::Track::PatVP:
        return s << "PatVP";
      case LHCb::Event::v1::Track::PatVeloUT:
        return s << "PatVeloUT";
      case LHCb::Event::v1::Track::PrForward:
        return s << "PrForward";
      case LHCb::Event::v1::Track::PrSeeding:
        return s << "PrSeeding";
      case LHCb::Event::v1::Track::PrMatch:
        return s << "PrMatch";
      case LHCb::Event::v1::Track::PrDownstream:
        return s << "PrDownstream";
      case LHCb::Event::v1::Track::PrVeloUT:
        return s << "PrVeloUT";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::History";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::FitHistory e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::FitUnknown:
        return s << "FitUnknown";
      case LHCb::Event::v1::Track::StdKalman:
        return s << "StdKalman";
      case LHCb::Event::v1::Track::BiKalman:
        return s << "BiKalman";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::FitHistory";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::Types e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::TypeUnknown:
        return s << "TypeUnknown";
      case LHCb::Event::v1::Track::Velo:
        return s << "Velo";
      case LHCb::Event::v1::Track::VeloR:
        return s << "VeloR";
      case LHCb::Event::v1::Track::Long:
        return s << "Long";
      case LHCb::Event::v1::Track::Upstream:
        return s << "Upstream";
      case LHCb::Event::v1::Track::Downstream:
        return s << "Downstream";
      case LHCb::Event::v1::Track::Ttrack:
        return s << "Ttrack";
      case LHCb::Event::v1::Track::Muon:
        return s << "Muon";
      case LHCb::Event::v1::Track::Calo:
        return s << "Calo";
      case LHCb::Event::v1::Track::TT:
        return s << "TT";
      case LHCb::Event::v1::Track::UT:
        return s << "UT";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::Types";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::PatRecStatus e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::PatRecStatusUnknown:
        return s << "PatRecStatusUnknown";
      case LHCb::Event::v1::Track::PatRecIDs:
        return s << "PatRecIDs";
      case LHCb::Event::v1::Track::PatRecMeas:
        return s << "PatRecMeas";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::PatRecStatus";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::FitStatus e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::FitStatusUnknown:
        return s << "FitStatusUnknown";
      case LHCb::Event::v1::Track::Fitted:
        return s << "Fitted";
      case LHCb::Event::v1::Track::FitFailed:
        return s << "FitFailed";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::FitStatus";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::Flags e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::FlagsUnknown:
        return s << "FlagsUnknown";
      case LHCb::Event::v1::Track::Backward:
        return s << "Backward";
      case LHCb::Event::v1::Track::Invalid:
        return s << "Invalid";
      case LHCb::Event::v1::Track::Clone:
        return s << "Clone";
      case LHCb::Event::v1::Track::Used:
        return s << "Used";
      case LHCb::Event::v1::Track::IPSelected:
        return s << "IPSelected";
      case LHCb::Event::v1::Track::PIDSelected:
        return s << "PIDSelected";
      case LHCb::Event::v1::Track::Selected:
        return s << "Selected";
      case LHCb::Event::v1::Track::L0Candidate:
        return s << "L0Candidate";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::Flags";
      }
    }

    inline std::ostream& operator<<( std::ostream& s, LHCb::Event::v1::Track::AdditionalInfo e ) {
      switch ( e ) {
      case LHCb::Event::v1::Track::AdditionalInfoUnknown:
        return s << "AdditionalInfoUnknown";
      case LHCb::Event::v1::Track::DC06Likelihood:
        return s << "DC06Likelihood";
      case LHCb::Event::v1::Track::PatQuality:
        return s << "PatQuality";
      case LHCb::Event::v1::Track::Cand1stQPat:
        return s << "Cand1stQPat";
      case LHCb::Event::v1::Track::Cand2ndQPat:
        return s << "Cand2ndQPat";
      case LHCb::Event::v1::Track::NCandCommonHits:
        return s << "NCandCommonHits";
      case LHCb::Event::v1::Track::Cand1stChi2Mat:
        return s << "Cand1stChi2Mat";
      case LHCb::Event::v1::Track::Cand2ndChi2Mat:
        return s << "Cand2ndChi2Mat";
      case LHCb::Event::v1::Track::DC06nExpectedVelo:
        return s << "DC06nExpectedVelo";
      case LHCb::Event::v1::Track::DC06nExpectedTT:
        return s << "DC06nExpectedTT";
      case LHCb::Event::v1::Track::DC06nExpectedIT:
        return s << "DC06nExpectedIT";
      case LHCb::Event::v1::Track::DC06nExpectedOT:
        return s << "DC06nExpectedOT";
      case LHCb::Event::v1::Track::MatchChi2:
        return s << "MatchChi2";
      case LHCb::Event::v1::Track::FitVeloChi2:
        return s << "FitVeloChi2";
      case LHCb::Event::v1::Track::FitVeloNDoF:
        return s << "FitVeloNDoF";
      case LHCb::Event::v1::Track::FitTChi2:
        return s << "FitTChi2";
      case LHCb::Event::v1::Track::FitTNDoF:
        return s << "FitTNDoF";
      case LHCb::Event::v1::Track::FitMatchChi2:
        return s << "FitMatchChi2";
      case LHCb::Event::v1::Track::FitFracUsedOTTimes:
        return s << "FitFracUsedOTTimes";
      case LHCb::Event::v1::Track::TsaLikelihood:
        return s << "TsaLikelihood";
      case LHCb::Event::v1::Track::CloneDist:
        return s << "CloneDist";
      case LHCb::Event::v1::Track::DC06GhostProbability:
        return s << "DC06GhostProbability";
      case LHCb::Event::v1::Track::nPRVeloRZExpect:
        return s << "nPRVeloRZExpect";
      case LHCb::Event::v1::Track::nPRVelo3DExpect:
        return s << "nPRVelo3DExpect";
      case LHCb::Event::v1::Track::AdditionalInfo201:
        return s << "AdditionalInfo201";
      case LHCb::Event::v1::Track::AdditionalInfo202:
        return s << "AdditionalInfo202";
      case LHCb::Event::v1::Track::MuonChi2perDoF:
        return s << "MuonChi2perDoF";
      case LHCb::Event::v1::Track::MuonMomentumPreSel:
        return s << "MuonMomentumPreSel";
      case LHCb::Event::v1::Track::MuonInAcceptance:
        return s << "MuonInAcceptance";
      case LHCb::Event::v1::Track::IsMuonLoose:
        return s << "IsMuonLoose";
      case LHCb::Event::v1::Track::IsMuon:
        return s << "IsMuon";
      case LHCb::Event::v1::Track::MuonDist2:
        return s << "MuonDist2";
      case LHCb::Event::v1::Track::MuonDLL:
        return s << "MuonDLL";
      case LHCb::Event::v1::Track::MuonNShared:
        return s << "MuonNShared";
      case LHCb::Event::v1::Track::MuonCLQuality:
        return s << "MuonCLQuality";
      case LHCb::Event::v1::Track::MuonCLArrival:
        return s << "MuonCLArrival";
      case LHCb::Event::v1::Track::IsMuonTight:
        return s << "IsMuonTight";
      default:
        return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Event::v1::Track::AdditionalInfo";
      }
    }

    // -----------------------------------------------------------------------------
    // end of class
    // -----------------------------------------------------------------------------

    // Including forward declarations

    inline LHCb::Event::v1::Track::Track( const History& history, const Types& trackType, const PatRecStatus& patStatus,
                                          const std::vector<LHCbID>& ids, const State& aState )
        : KeyedObject<int>()
        , m_chi2PerDoF( 0.0 )
        , m_nDoF( 0 )
        , m_likelihood( 999 )
        , m_ghostProbability( 999 )
        , m_flags( 0 )
        , m_lhcbIDs( std::move( ids ) ) {

      addToStates( aState );
      setHistory( history );
      setType( trackType );
      setPatRecStatus( patStatus );
      std::sort( m_lhcbIDs.begin(), m_lhcbIDs.end() );
    }

    inline LHCb::Event::v1::Track::Track( const History& history, const Types& trackType,
                                          const PatRecStatus& patStatus )
        : KeyedObject<int>()
        , m_chi2PerDoF( 0.0 )
        , m_nDoF( 0 )
        , m_likelihood( 999 )
        , m_ghostProbability( 999 )
        , m_flags( 0 ) {

      setHistory( history );
      setType( trackType );
      setPatRecStatus( patStatus );
    }

    inline LHCb::Event::v1::Track::Track( const Track& track )
        : KeyedObject<int>()
        , m_chi2PerDoF( 0.0 )
        , m_nDoF( 0 )
        , m_likelihood( 999 )
        , m_ghostProbability( 999 )
        , m_flags( 0 )
        , m_fitResult( nullptr )
        , m_ancestors() {

      this->copy( track );
    }

    inline LHCb::Event::v1::Track::Track( const Track& track, const int key )
        : KeyedObject<int>( key )
        , m_chi2PerDoF( 0.0 )
        , m_nDoF( 0 )
        , m_likelihood( 999 )
        , m_ghostProbability( 999 )
        , m_flags( 0 )
        , m_lhcbIDs()
        , m_states()
        , m_fitResult()
        , m_ancestors() {

      this->copy( track );
    }

    inline LHCb::Event::v1::Track::~Track() {

      std::for_each( m_states.begin(), m_states.end(), []( auto& i ) { delete i; } );
    }

    inline const CLID& LHCb::Event::v1::Track::clID() const { return LHCb::Event::v1::Track::classID(); }

    inline const CLID& LHCb::Event::v1::Track::classID() { return CLID_Track; }

    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::History>&
    LHCb::Event::v1::Track::s_HistoryTypMap() {
      static const GaudiUtils::VectorMap<std::string, History> m = {{"HistoryUnknown", HistoryUnknown},
                                                                    {"TrackIdealPR", TrackIdealPR},
                                                                    {"TrackSeeding", TrackSeeding},
                                                                    {"PatVelo", PatVelo},
                                                                    {"PatVeloTT", PatVeloTT},
                                                                    {"TrackVeloTT", TrackVeloTT},
                                                                    {"PatForward", PatForward},
                                                                    {"TrackMatching", TrackMatching},
                                                                    {"PatKShort", PatKShort},
                                                                    {"TsaTrack", TsaTrack},
                                                                    {"MuonID", MuonID},
                                                                    {"PatVeloR", PatVeloR},
                                                                    {"PatVeloGeneric", PatVeloGeneric},
                                                                    {"PatVeloGeneral", PatVeloGeneral},
                                                                    {"PatVeloOpen", PatVeloOpen},
                                                                    {"PatSeeding", PatSeeding},
                                                                    {"PatMatch", PatMatch},
                                                                    {"PatDownstream", PatDownstream},
                                                                    {"TTGeneric", TTGeneric},
                                                                    {"PatVPCheated", PatVPCheated},
                                                                    {"HLTImportedTrack", HLTImportedTrack},
                                                                    {"PatFastVelo", PatFastVelo},
                                                                    {"PatVP", PatVP},
                                                                    {"PatVeloUT", PatVeloUT},
                                                                    {"PrForward", PrForward},
                                                                    {"PrSeeding", PrSeeding},
                                                                    {"PrMatch", PrMatch},
                                                                    {"PrDownstream", PrDownstream},
                                                                    {"PrVeloUT", PrVeloUT}};
      return m;
    }

    inline LHCb::Event::v1::Track::History LHCb::Event::v1::Track::HistoryToType( const std::string& aName ) {
      auto iter = s_HistoryTypMap().find( aName );
      return iter != s_HistoryTypMap().end() ? iter->second : HistoryUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::HistoryToString( int aEnum ) {
      static const std::string s_HistoryUnknown = "HistoryUnknown";
      auto                     iter             = std::find_if( s_HistoryTypMap().begin(), s_HistoryTypMap().end(),
                                [&]( const std::pair<const std::string, History>& i ) { return i.second == aEnum; } );
      return iter != s_HistoryTypMap().end() ? iter->first : s_HistoryUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::FitHistory>&
    LHCb::Event::v1::Track::s_FitHistoryTypMap() {
      static const GaudiUtils::VectorMap<std::string, FitHistory> m = {
          {"FitUnknown", FitUnknown}, {"StdKalman", StdKalman}, {"BiKalman", BiKalman}};
      return m;
    }

    inline LHCb::Event::v1::Track::FitHistory LHCb::Event::v1::Track::FitHistoryToType( const std::string& aName ) {
      auto iter = s_FitHistoryTypMap().find( aName );
      return iter != s_FitHistoryTypMap().end() ? iter->second : FitUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::FitHistoryToString( int aEnum ) {
      static const std::string s_FitUnknown = "FitUnknown";
      auto                     iter =
          std::find_if( s_FitHistoryTypMap().begin(), s_FitHistoryTypMap().end(),
                        [&]( const std::pair<const std::string, FitHistory>& i ) { return i.second == aEnum; } );
      return iter != s_FitHistoryTypMap().end() ? iter->first : s_FitUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::Types>&
    LHCb::Event::v1::Track::s_TypesTypMap() {
      static const GaudiUtils::VectorMap<std::string, Types> m = {{"TypeUnknown", TypeUnknown},
                                                                  {"Velo", Velo},
                                                                  {"VeloR", VeloR},
                                                                  {"Long", Long},
                                                                  {"Upstream", Upstream},
                                                                  {"Downstream", Downstream},
                                                                  {"Ttrack", Ttrack},
                                                                  {"Muon", Muon},
                                                                  {"Calo", Calo},
                                                                  {"TT", TT},
                                                                  {"UT", UT}};
      return m;
    }

    inline LHCb::Event::v1::Track::Types LHCb::Event::v1::Track::TypesToType( const std::string& aName ) {
      auto iter = s_TypesTypMap().find( aName );
      return iter != s_TypesTypMap().end() ? iter->second : TypeUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::TypesToString( int aEnum ) {
      static const std::string s_TypeUnknown = "TypeUnknown";
      auto                     iter          = std::find_if( s_TypesTypMap().begin(), s_TypesTypMap().end(),
                                [&]( const std::pair<const std::string, Types>& i ) { return i.second == aEnum; } );
      return iter != s_TypesTypMap().end() ? iter->first : s_TypeUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::PatRecStatus>&
    LHCb::Event::v1::Track::s_PatRecStatusTypMap() {
      static const GaudiUtils::VectorMap<std::string, PatRecStatus> m = {
          {"PatRecStatusUnknown", PatRecStatusUnknown}, {"PatRecIDs", PatRecIDs}, {"PatRecMeas", PatRecMeas}};
      return m;
    }

    inline LHCb::Event::v1::Track::PatRecStatus LHCb::Event::v1::Track::PatRecStatusToType( const std::string& aName ) {
      auto iter = s_PatRecStatusTypMap().find( aName );
      return iter != s_PatRecStatusTypMap().end() ? iter->second : PatRecStatusUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::PatRecStatusToString( int aEnum ) {
      static const std::string s_PatRecStatusUnknown = "PatRecStatusUnknown";
      auto                     iter =
          std::find_if( s_PatRecStatusTypMap().begin(), s_PatRecStatusTypMap().end(),
                        [&]( const std::pair<const std::string, PatRecStatus>& i ) { return i.second == aEnum; } );
      return iter != s_PatRecStatusTypMap().end() ? iter->first : s_PatRecStatusUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::FitStatus>&
    LHCb::Event::v1::Track::s_FitStatusTypMap() {
      static const GaudiUtils::VectorMap<std::string, FitStatus> m = {
          {"FitStatusUnknown", FitStatusUnknown}, {"Fitted", Fitted}, {"FitFailed", FitFailed}};
      return m;
    }

    inline LHCb::Event::v1::Track::FitStatus LHCb::Event::v1::Track::FitStatusToType( const std::string& aName ) {
      auto iter = s_FitStatusTypMap().find( aName );
      return iter != s_FitStatusTypMap().end() ? iter->second : FitStatusUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::FitStatusToString( int aEnum ) {
      static const std::string s_FitStatusUnknown = "FitStatusUnknown";
      auto                     iter = std::find_if( s_FitStatusTypMap().begin(), s_FitStatusTypMap().end(),
                                [&]( const std::pair<const std::string, FitStatus>& i ) { return i.second == aEnum; } );
      return iter != s_FitStatusTypMap().end() ? iter->first : s_FitStatusUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::Flags>&
    LHCb::Event::v1::Track::s_FlagsTypMap() {
      static const GaudiUtils::VectorMap<std::string, Flags> m = {{"FlagsUnknown", FlagsUnknown},
                                                                  {"Backward", Backward},
                                                                  {"Invalid", Invalid},
                                                                  {"Clone", Clone},
                                                                  {"Used", Used},
                                                                  {"IPSelected", IPSelected},
                                                                  {"PIDSelected", PIDSelected},
                                                                  {"Selected", Selected},
                                                                  {"L0Candidate", L0Candidate}};
      return m;
    }

    inline LHCb::Event::v1::Track::Flags LHCb::Event::v1::Track::FlagsToType( const std::string& aName ) {
      auto iter = s_FlagsTypMap().find( aName );
      return iter != s_FlagsTypMap().end() ? iter->second : FlagsUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::FlagsToString( int aEnum ) {
      static const std::string s_FlagsUnknown = "FlagsUnknown";
      auto                     iter           = std::find_if( s_FlagsTypMap().begin(), s_FlagsTypMap().end(),
                                [&]( const std::pair<const std::string, Flags>& i ) { return i.second == aEnum; } );
      return iter != s_FlagsTypMap().end() ? iter->first : s_FlagsUnknown;
    }
    inline const GaudiUtils::VectorMap<std::string, LHCb::Event::v1::Track::AdditionalInfo>&
    LHCb::Event::v1::Track::s_AdditionalInfoTypMap() {
      static const GaudiUtils::VectorMap<std::string, AdditionalInfo> m = {
          {"AdditionalInfoUnknown", AdditionalInfoUnknown},
          {"DC06Likelihood", DC06Likelihood},
          {"PatQuality", PatQuality},
          {"Cand1stQPat", Cand1stQPat},
          {"Cand2ndQPat", Cand2ndQPat},
          {"NCandCommonHits", NCandCommonHits},
          {"Cand1stChi2Mat", Cand1stChi2Mat},
          {"Cand2ndChi2Mat", Cand2ndChi2Mat},
          {"DC06nExpectedVelo", DC06nExpectedVelo},
          {"DC06nExpectedTT", DC06nExpectedTT},
          {"DC06nExpectedIT", DC06nExpectedIT},
          {"DC06nExpectedOT", DC06nExpectedOT},
          {"MatchChi2", MatchChi2},
          {"FitVeloChi2", FitVeloChi2},
          {"FitVeloNDoF", FitVeloNDoF},
          {"FitTChi2", FitTChi2},
          {"FitTNDoF", FitTNDoF},
          {"FitMatchChi2", FitMatchChi2},
          {"FitFracUsedOTTimes", FitFracUsedOTTimes},
          {"TsaLikelihood", TsaLikelihood},
          {"CloneDist", CloneDist},
          {"DC06GhostProbability", DC06GhostProbability},
          {"nPRVeloRZExpect", nPRVeloRZExpect},
          {"nPRVelo3DExpect", nPRVelo3DExpect},
          {"AdditionalInfo201", AdditionalInfo201},
          {"AdditionalInfo202", AdditionalInfo202},
          {"MuonChi2perDoF", MuonChi2perDoF},
          {"MuonMomentumPreSel", MuonMomentumPreSel},
          {"MuonInAcceptance", MuonInAcceptance},
          {"IsMuonLoose", IsMuonLoose},
          {"IsMuon", IsMuon},
          {"MuonDist2", MuonDist2},
          {"MuonDLL", MuonDLL},
          {"MuonNShared", MuonNShared},
          {"MuonCLQuality", MuonCLQuality},
          {"MuonCLArrival", MuonCLArrival},
          {"IsMuonTight", IsMuonTight}};
      return m;
    }

    inline LHCb::Event::v1::Track::AdditionalInfo
    LHCb::Event::v1::Track::AdditionalInfoToType( const std::string& aName ) {
      auto iter = s_AdditionalInfoTypMap().find( aName );
      return iter != s_AdditionalInfoTypMap().end() ? iter->second : AdditionalInfoUnknown;
    }

    inline const std::string& LHCb::Event::v1::Track::AdditionalInfoToString( int aEnum ) {
      static const std::string s_AdditionalInfoUnknown = "AdditionalInfoUnknown";
      auto                     iter =
          std::find_if( s_AdditionalInfoTypMap().begin(), s_AdditionalInfoTypMap().end(),
                        [&]( const std::pair<const std::string, AdditionalInfo>& i ) { return i.second == aEnum; } );
      return iter != s_AdditionalInfoTypMap().end() ? iter->first : s_AdditionalInfoUnknown;
    }

    inline double LHCb::Event::v1::Track::chi2PerDoF() const { return m_chi2PerDoF; }

    inline void LHCb::Event::v1::Track::setChi2PerDoF( double value ) { m_chi2PerDoF = value; }

    inline int LHCb::Event::v1::Track::nDoF() const { return m_nDoF; }

    inline void LHCb::Event::v1::Track::setNDoF( int value ) { m_nDoF = value; }

    inline double LHCb::Event::v1::Track::likelihood() const { return m_likelihood; }

    inline void LHCb::Event::v1::Track::setLikelihood( double value ) { m_likelihood = value; }

    inline double LHCb::Event::v1::Track::ghostProbability() const { return m_ghostProbability; }

    inline void LHCb::Event::v1::Track::setGhostProbability( double value ) { m_ghostProbability = value; }

    inline unsigned int LHCb::Event::v1::Track::flags() const { return m_flags; }

    inline void LHCb::Event::v1::Track::setFlags( unsigned int value ) { m_flags = value; }

    inline LHCb::Event::v1::Track::Types LHCb::Event::v1::Track::type() const {
      return ( LHCb::Event::v1::Track::Types )( ( m_flags & typeMask ) >> typeBits );
    }

    inline void LHCb::Event::v1::Track::setType( const Types& value ) {
      auto val = (unsigned int)value;
      m_flags &= ~typeMask;
      m_flags |= ( ( ( (unsigned int)val ) << typeBits ) & typeMask );
    }

    inline LHCb::Event::v1::Track::History LHCb::Event::v1::Track::history() const {
      return ( LHCb::Event::v1::Track::History )( ( m_flags & historyMask ) >> historyBits );
    }

    inline void LHCb::Event::v1::Track::setHistory( const History& value ) {
      auto val = (unsigned int)value;
      m_flags &= ~historyMask;
      m_flags |= ( ( ( (unsigned int)val ) << historyBits ) & historyMask );
    }

    inline LHCb::Event::v1::Track::FitHistory LHCb::Event::v1::Track::fitHistory() const {
      return ( LHCb::Event::v1::Track::FitHistory )( ( m_flags & fitHistoryMask ) >> fitHistoryBits );
    }

    inline void LHCb::Event::v1::Track::setFitHistory( const FitHistory& value ) {
      auto val = (unsigned int)value;
      m_flags &= ~fitHistoryMask;
      m_flags |= ( ( ( (unsigned int)val ) << fitHistoryBits ) & fitHistoryMask );
    }

    inline LHCb::Event::v1::Track::PatRecStatus LHCb::Event::v1::Track::patRecStatus() const {
      return ( LHCb::Event::v1::Track::PatRecStatus )( ( m_flags & patRecStatusMask ) >> patRecStatusBits );
    }

    inline void LHCb::Event::v1::Track::setPatRecStatus( const PatRecStatus& value ) {
      auto val = (unsigned int)value;
      m_flags &= ~patRecStatusMask;
      m_flags |= ( ( ( (unsigned int)val ) << patRecStatusBits ) & patRecStatusMask );
    }

    inline LHCb::Event::v1::Track::FitStatus LHCb::Event::v1::Track::fitStatus() const {
      return ( LHCb::Event::v1::Track::FitStatus )( ( m_flags & fitStatusMask ) >> fitStatusBits );
    }

    inline void LHCb::Event::v1::Track::setFitStatus( const FitStatus& value ) {
      auto val = (unsigned int)value;
      m_flags &= ~fitStatusMask;
      m_flags |= ( ( ( (unsigned int)val ) << fitStatusBits ) & fitStatusMask );
    }

    inline LHCb::Event::v1::Track::Flags LHCb::Event::v1::Track::flag() const {
      return ( LHCb::Event::v1::Track::Flags )( ( m_flags & flagMask ) >> flagBits );
    }

    inline unsigned int LHCb::Event::v1::Track::specific() const {
      return (unsigned int)( ( m_flags & specificMask ) >> specificBits );
    }

    inline void LHCb::Event::v1::Track::setSpecific( unsigned int value ) {
      auto val = (unsigned int)value;
      m_flags &= ~specificMask;
      m_flags |= ( ( ( (unsigned int)val ) << specificBits ) & specificMask );
    }

    inline const std::vector<LHCbID>& LHCb::Event::v1::Track::lhcbIDs() const { return m_lhcbIDs; }

    inline const std::vector<State*>& LHCb::Event::v1::Track::states() const { return m_states; }

    inline std::vector<State*>& LHCb::Event::v1::Track::states() { return m_states; }

    inline const LHCb::Event::v1::Track::ExtraInfo& LHCb::Event::v1::Track::extraInfo() const { return m_extraInfo; }

    inline void LHCb::Event::v1::Track::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

    inline const SmartRefVector<Track>& LHCb::Event::v1::Track::ancestors() const { return m_ancestors; }

    inline SmartRefVector<Track>& LHCb::Event::v1::Track::ancestors() { return m_ancestors; }

    inline void LHCb::Event::v1::Track::addToAncestors( const SmartRef<Track>& value ) {
      m_ancestors.push_back( value );
    }

    inline void LHCb::Event::v1::Track::addToAncestors( const Track* value ) { m_ancestors.push_back( value ); }

    inline void LHCb::Event::v1::Track::removeFromAncestors( const SmartRef<Track>& value ) {
      auto i = std::remove( m_ancestors.begin(), m_ancestors.end(), value );
      m_ancestors.erase( i, m_ancestors.end() );
    }

    inline void LHCb::Event::v1::Track::clearAncestors() { m_ancestors.clear(); }

    inline void LHCb::Event::v1::Track::positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom,
                                                             Gaudi::SymMatrix6x6& cov6D ) const {

      firstState().positionAndMomentum( pos, mom, cov6D );
    }

    inline void LHCb::Event::v1::Track::positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const {

      firstState().positionAndMomentum( pos, mom );
    }

    inline Gaudi::XYZPoint LHCb::Event::v1::Track::position() const { return firstState().position(); }

    inline void LHCb::Event::v1::Track::position( Gaudi::XYZPoint& pos, Gaudi::SymMatrix3x3& errPos ) const {

      pos    = firstState().position();
      errPos = firstState().errPosition();
    }

    inline Gaudi::XYZVector LHCb::Event::v1::Track::slopes() const { return firstState().slopes(); }

    inline void LHCb::Event::v1::Track::slopes( Gaudi::XYZVector& slopes, Gaudi::SymMatrix3x3& errSlopes ) const {

      slopes    = firstState().slopes();
      errSlopes = firstState().errSlopes();
    }

    inline Gaudi::XYZVector LHCb::Event::v1::Track::momentum() const { return this->firstState().momentum(); }

    inline double LHCb::Event::v1::Track::p() const { return this->firstState().p(); }

    inline double LHCb::Event::v1::Track::pt() const { return this->firstState().pt(); }

    inline double LHCb::Event::v1::Track::pseudoRapidity() const { return this->slopes().eta(); }

    inline double LHCb::Event::v1::Track::phi() const { return this->slopes().phi(); }

    inline void LHCb::Event::v1::Track::momentum( Gaudi::XYZVector& mom, Gaudi::SymMatrix3x3& errMom ) const {

      mom    = firstState().momentum();
      errMom = firstState().errMomentum();
    }

    inline void LHCb::Event::v1::Track::posMomCovariance( Gaudi::SymMatrix6x6& cov6D ) const {

      cov6D = firstState().posMomCovariance();
    }

    inline State& LHCb::Event::v1::Track::firstState() {

      // check at least the "first state" exists
      if ( UNLIKELY( m_states.empty() ) )
        throw GaudiException( "first state not defined!", "Track.h", StatusCode::FAILURE );
      return *m_states[0];
    }

    inline const State& LHCb::Event::v1::Track::firstState() const {

      // check at least the "first state" exists
      if ( UNLIKELY( m_states.empty() ) )
        throw GaudiException( "first state not defined!", "Track.h", StatusCode::FAILURE );
      return *m_states[0];
    }

    inline int LHCb::Event::v1::Track::charge() const {

      double qP = firstState().qOverP();
      return UNLIKELY( std::abs( qP ) < TrackParameters::lowTolerance ) ? 0 : qP < 0 ? -1 : +1;
    }

    inline double LHCb::Event::v1::Track::chi2() const { return ( m_chi2PerDoF * double( m_nDoF ) ); }

    inline void LHCb::Event::v1::Track::setChi2AndDoF( double chi2, int ndof ) {

      m_chi2PerDoF = ( ndof != 0 ) ? chi2 / ( (double)( ndof ) ) : 0.0;
      m_nDoF       = ndof;
    }

    inline unsigned int LHCb::Event::v1::Track::nStates() const { return m_states.size(); }

    inline unsigned int LHCb::Event::v1::Track::nLHCbIDs() const { return m_lhcbIDs.size(); }

    inline bool LHCb::Event::v1::Track::addToLhcbIDs( span<const LHCbID> ids ) {

      LHCbIDContainer copy( ids.begin(), ids.end() );
      std::sort( copy.begin(), copy.end() );
      return addSortedToLhcbIDs( copy );
    }

    inline void LHCb::Event::v1::Track::setLhcbIDs( span<const LHCbID> ids ) {

      m_lhcbIDs.assign( ids.begin(), ids.end() );
      std::sort( m_lhcbIDs.begin(), m_lhcbIDs.end() );
    }

    inline void LHCb::Event::v1::Track::setSortedLhcbIDs( span<const LHCbID> ids ) {

      m_lhcbIDs.assign( ids.begin(), ids.end() );
      assert( std::is_sorted( begin( m_lhcbIDs ), end( m_lhcbIDs ) ) );
    }

    inline void LHCb::Event::v1::Track::setSortedLhcbIDs( LHCbIDContainer&& value ) {

      m_lhcbIDs = std::move( value );
      assert( std::is_sorted( begin( m_lhcbIDs ), end( m_lhcbIDs ) ) );
    }

    inline bool LHCb::Event::v1::Track::containsLhcbIDs( const Track& track ) const {

      return containsLhcbIDs( track.lhcbIDs() );
    }

    inline bool LHCb::Event::v1::Track::containsLhcbIDs( span<const LHCbID> ids ) const {

      return std::includes( m_lhcbIDs.begin(), m_lhcbIDs.end(), ids.begin(), ids.end() );
    }

    inline void LHCb::Event::v1::Track::addToAncestors( const Track& ancestor ) {

      m_ancestors.push_back( const_cast<Track*>( &ancestor ) );
    }

    inline bool LHCb::Event::v1::Track::checkType( const Types& value ) const { return type() == value; }

    inline bool LHCb::Event::v1::Track::checkHistory( const History& value ) const { return history() == value; }

    inline bool LHCb::Event::v1::Track::checkFitHistory( const FitHistory& value ) const {

      return fitHistory() == value;
    }

    inline bool LHCb::Event::v1::Track::checkPatRecStatus( const PatRecStatus& value ) const {

      return patRecStatus() == value;
    }

    inline bool LHCb::Event::v1::Track::checkFitStatus( const FitStatus& value ) const { return fitStatus() == value; }

    inline void LHCb::Event::v1::Track::setFlag( unsigned int flag, bool ok ) {

      unsigned int val = ( ( (unsigned int)flag ) << flagBits ) & flagMask;
      if ( ok )
        m_flags |= val;
      else
        m_flags &= ~val;
    }

    inline bool LHCb::Event::v1::Track::checkFlag( const Flags& flag ) const {

      unsigned int val = ( (unsigned int)flag << flagBits );
      return ( 0 != ( m_flags & flagMask & val ) );
    }

    inline bool LHCb::Event::v1::Track::hasT() const {

      const auto t = type();
      return t == Track::Types::Ttrack || t == Track::Types::Downstream || t == Track::Types::Long;
    }

    inline bool LHCb::Event::v1::Track::hasVelo() const {

      const auto t = type();
      return t == Track::Types::Velo || t == Track::Types::VeloR || t == Track::Types::Upstream ||
             t == Track::Types::Long;
    }

    inline bool LHCb::Event::v1::Track::hasTT() const {

      const auto t = type();
      return t == Track::Types::Downstream || t == Track::Types::Upstream || t == Track::Types::Long;
    }

    inline bool LHCb::Event::v1::Track::hasUT() const {

      const auto t = type();
      return t == Track::Types::Downstream || t == Track::Types::Upstream || t == Track::Types::Long;
    }

  } // namespace v1
} // namespace LHCb::Event

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
REGISTER_HEADER( LHCb::Event::v1::Track, "Event/Track_v1.h" );