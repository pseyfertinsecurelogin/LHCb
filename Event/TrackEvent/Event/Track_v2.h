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
#ifndef TrackEvent_v2_H
#define TrackEvent_v2_H 1

// Include files
#include "Event/Measurement.h"
#include "Event/Node.h"
#include "Event/State.h"
#include "Event/TrackFitResult.h"
#include "Event/TrackParameters.h"
#include "Event/TrackTags.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Range.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/LHCbID.h"
#include "Kernel/PolymorphicValue.h"
#include "Kernel/STLExtensions.h"
#include <algorithm>
#include <ostream>
#include <type_traits>
#include <vector>

/**
 *
 * Track v2 is the base class for offline and online tracks.
 *
 * @author Sascha Stahl
 *
 */

namespace LHCb::Event
{
  namespace v2
  {
    // Namespace for locations in TDS
    namespace TrackLocation
    {
      inline std::string const Default             = "Rec/Track/Best";
      inline std::string const Ideal               = "MC/Track/Ideal";
      inline std::string const Seed                = "Rec/Track/Seed";
      inline std::string const RZVelo              = "Rec/Track/RZVelo";
      inline std::string const Velo                = "Rec/Track/Velo";
      inline std::string const Forward             = "Rec/Track/Forward";
      inline std::string const VeloTT              = "Rec/Track/VeloTT";
      inline std::string const TT                  = "Rec/Track/TT";
      inline std::string const Match               = "Rec/Track/Match";
      inline std::string const Downstream          = "Rec/Track/Downstream";
      inline std::string const Tsa                 = "Rec/Track/Tsa";
      inline std::string const Muon                = "Rec/Track/Muon";
      inline std::string const VP                  = "Rec/Track/VP";
      inline std::string const VPTT                = "Rec/Track/VPTT";
      inline std::string const VeloUT              = "Rec/Track/VeloUT";
      inline std::string const UT                  = "Rec/Track/UT";
      inline std::string const CaloCosmicsForward  = "Calo/Track/Forward";
      inline std::string const CaloCosmicsBackward = "Calo/Track/Backward";
      inline std::string const HltImport           = "Red/Track/HLTImport";
    }

    namespace details
    {
      template <std::size_t nbits, std::size_t offset, typename T>
      constexpr T makeMask()
      {
        static_assert( std::numeric_limits<T>::is_integer );
        static_assert( !std::numeric_limits<T>::is_signed );
        static_assert( std::numeric_limits<T>::radix == 2 );

        constexpr auto maxbits = std::numeric_limits<T>::digits;

        static_assert( nbits <= maxbits );
        static_assert( offset <= maxbits - nbits );

        return ( ~T{0} >> ( maxbits - nbits ) ) << offset;
      }
      template <std::size_t n_bits, std::size_t offset, typename UInt = std::uint32_t>
      constexpr auto mask_v = details::makeMask<n_bits, offset, UInt>();

      constexpr int trailing_zeros( uint32_t v )
      {
        // from http://graphics.stanford.edu/~seander/bithacks.html
        constexpr auto tbl = std::array{0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
                                        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
        return tbl[( static_cast<uint32_t>( ( v & -v ) * 0x077CB531U ) ) >> 27];
      }

      template <uint32_t mask>
      constexpr void setBits( uint32_t& target, uint32_t value )
      {
        assert( ( value & ~( mask >> trailing_zeros( mask ) ) ) == 0 );
        target &= ~mask;
        target |= ( value << trailing_zeros( mask ) ) & mask;
      }

      template <uint32_t mask>
      constexpr uint32_t getBits( uint32_t val )
      {
        return ( val & mask ) >> trailing_zeros( mask );
      }
    }

    class Track final
    {
    public:
      /// Vector of additional information
      typedef GaudiUtils::VectorMap<int, double> ExtraInfo;
      /// Container for LHCbIDs on track
      typedef std::vector<LHCbID> LHCbIDContainer;
      /// Container for Measurements on track
      typedef std::vector<Measurement const*> MeasurementContainer;
      /// Container for States on track
      typedef std::vector<State> StateContainer;
      /// Range of pointers to nodes on track. For non-const access, use fitresult.
      typedef Gaudi::Range_<std::vector<Node const*>> ConstNodeRange;
      struct Chi2PerDoF {
        double chi2PerDoF = 0.0;
        int    nDoF       = 0;
        auto   chi2() const { return chi2PerDoF * nDoF; };
      };

      /// Track history enumerations: each entry corresponds to the name of the pattern recognition algorithm that
      /// produced the Track
      enum class History {
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
      /// conversion of string to enum for type History
      friend StatusCode parse(History& e, std::string const& name);
      /// conversion to string for enum type History
      friend std::string const& toString( History const flag );

      /// Track fit history enumerations
      enum class FitHistory {
        FitUnknown = 0, // track not fitted yet (fit history not set)
        StdKalman,      // track fitted with the standard Kalman fitter
        BiKalman        // track fitted with the bi-directional Kalman fitter
      };
      /// conversion of string to enum for type FitHistory
      friend StatusCode parse(FitHistory& e, std::string const& name);
      /// conversion to string for enum type FitHistory
      friend std::string const& toString( FitHistory const flag );

      /// Track type enumerations
      enum class Types {
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
      /// conversion of string to enum for type Types
      friend StatusCode parse(Types& e, std::string const& name);
      /// conversion to string for enum type Types
      friend std::string const& toString( Types const flag );

      /// Track pattern recognition status flag enumerations: The flag specifies in which state of the pattern
      /// recognition phase the track is. The status flag is set by the relevant algorithms
      enum class PatRecStatus {
        PatRecStatusUnknown = 0, // track in an undefined PR status
        PatRecIDs,               // pattern recognition track with LHCbIDs
        PatRecMeas               // pattern recognition track with Measurements added
      };
      /// conversion of string to enum for type PatRecStatus
      friend StatusCode parse(PatRecStatus& e, std::string const& name);
      /// conversion to string for enum type PatRecStatus
      friend std::string const& toString( PatRecStatus const flag );

      /// Track fitting status flag enumerations: The flag specifies in which state of the fitting phase the track is.
      /// The status flag is set by the relevant algorithms
      enum class FitStatus {
        FitStatusUnknown = 0, // track in an undefined fitting status
        Fitted,               // fitted track
        FitFailed             // track for which the track fit failed
      };
      /// conversion of string to enum for type FitStatus
      friend StatusCode parse(FitStatus& e, std::string const& name);
      /// conversion to string for enum type FitStatus
      friend std::string const& toString( FitStatus const flag );

      /// Track general flags enumerations
      enum class Flags {
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
      /// conversion of string to enum for type Flags
      friend StatusCode parse(Flags& e, std::string const& name);
      /// conversion to string for enum type Flags
      friend std::string const& toString( Flags const flag );

      /// Additional information assigned to this Track by pattern recognition
      enum class AdditionalInfo {
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
      /// conversion of string to enum for type AdditionalInfo
      friend StatusCode parse(AdditionalInfo& e, std::string const& name);
      /// conversion to string for enum type AdditionalInfo
      friend std::string const& toString( AdditionalInfo const flag );

      /// Retrieve the position and momentum vectors and the corresponding 6D covariance matrix (pos:0->2,mom:3-5) at
      /// the first state
      std::tuple<Gaudi::XYZPoint, Gaudi::XYZVector, Gaudi::SymMatrix6x6> positionMomentumAndError() const
      {
        return {firstState().position(), firstState().momentum(), firstState().posMomCovariance()};
      };

      /// Retrieve the position and momentum vectors at the first state
      std::pair<Gaudi::XYZPoint, Gaudi::XYZVector> positionAndMomentum() const
      {
        return {firstState().position(), firstState().momentum()};
      };

      /// Retrieve the 3D-position vector at the first state
      Gaudi::XYZPoint position() const { return firstState().position(); };

      /// Retrieve the 3D-position (+ errors) at the first state
      std::pair<Gaudi::XYZPoint, Gaudi::SymMatrix3x3> positionAndError() const
      {
        return {firstState().position(), firstState().errPosition()};
      };

      /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) and errors at the first state
      Gaudi::XYZVector slopes() const { return firstState().slopes(); };

      /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) at the first state
      std::pair<Gaudi::XYZVector, Gaudi::SymMatrix3x3> slopesAndError() const
      {
        return {firstState().slopes(), firstState().errSlopes()};
      };

      /// Retrieve the momentum vector at the first state
      Gaudi::XYZVector momentum() const { return firstState().momentum(); };

      /// Retrieve the momentum at the first state
      auto p() const { return firstState().p(); };

      /// Retrieve the transverse momentum at the first state
      auto pt() const { return firstState().pt(); };

      /// Retrieve the pseudorapidity at the first state
      auto pseudoRapidity() const { return slopes().eta(); };

      /// Retrieve the phi at the first state
      auto phi() const { return slopes().phi(); };

      /// Retrieve the momentum vector (and errors) at the first state
      std::pair<Gaudi::XYZVector, Gaudi::SymMatrix3x3> momentumAndError() const
      {
        return {firstState().momentum(), firstState().errMomentum()};
      };

      /// Retrieve the 6D (x,y,z,px,py,pz) covariance matrix at the first state
      Gaudi::SymMatrix6x6 posMomCovariance() const { return firstState().posMomCovariance(); };

      /// Retrieve the first state on the track
      State const& firstState() const { return m_states.at( 0 ); };

      /// Retrieve the charge assigned to the track
      int charge() const;

      /// Retrieve the Chi^2 of the track (fit)
      auto chi2() const { return m_chi2PerDoF.chi2(); };

      /// Probability of chi2^2 of the track
      double probChi2() const;

      /// Retrieve the number of states on the track
      unsigned int nStates() const { return m_states.size(); };

      /// Sets POverQ for all states
      void setQOverPInAllStates( float const qop );

      /// Sets POverQ adn ErrQOverP2 for all states
      void setQOverPAndErrInAllStates( float const qop, float const err );

      /// Retrieve vector with measurements on the track
      MeasurementContainer measurements() const;

      /// Const retrieve the nodes on the track
      ConstNodeRange nodes() const;

      /// Add a State to the list of States associated to the track
      void addToStates( State const& state );

      /// Add a set of states to the track.
      void addToStates( span<State const> states, Tag::Unordered_tag );

      /// Add a set of sorted states by increasing Z to the track.
      void addToStates( span<State const> states, Tag::Sorted_tag );

      /// Clear the State vector
      void clearStates() { m_states.clear(); };

      /// Retrieve the reference to the state closest to the given z-position
      State& closestState( double const z );

      /// Retrieve the reference to the state closest to the given z-position
      State const& closestState( double const z ) const;

      /// Retrieve the reference to the state closest to the given plane
      State const& closestState( Gaudi::Plane3D const& plane ) const;

      /// Check the existence of a state at a certain predefined location (see the Location enum in State.h)
      bool hasStateAt( State::Location const& location ) const { return stateAt( location ) != nullptr; };

      /// Retrieve the state at a certain predefined location (see the Location enum in State.h)
      State* stateAt( State::Location const& location );

      /// Retrieve the const state at a certain predefined location (see the Location enum in State.h)
      State const* stateAt( State::Location const& location ) const;

      /// Retrieve the number of LHCbIDs on the track
      unsigned int nLHCbIDs() const { return m_lhcbIDs.size(); };

      /// Retrieve the number of Measurements on the track
      unsigned int nMeasurements() const { return m_fitResult ? m_fitResult->nActiveMeasurements() : 0; };

      /// Add an LHCbID to the list of LHCbIDs associated to the track. Return true if LHCbID was not yet on track.
      bool addToLhcbIDs( LHCbID const& value );

      template <
          typename S,
          typename = std::enable_if_t<std::is_same_v<S, Tag::Sorted_tag> || std::is_same_v<S, Tag::Unordered_tag>>>
      bool addToLhcbIDs( span<LHCbID const> ids, S /**/ );

      /// Sets the list of LHCbIDs associated to this track. The input vector will be sorted.
      void setLhcbIDs( span<LHCbID const> ids, Tag::Unordered_tag );

      /// Sets the list of LHCbIDs associated to this track. The input vector must be sorted.
      void setLhcbIDs( span<LHCbID const> ids, Tag::Sorted_tag );

      /// Sets the list of LHCbIDs associated to this track. The input vector must be sorted.
      void setLhcbIDs( LHCbIDContainer&& value, Tag::Sorted_tag );

      /// Returns true if the LHCbIDs of track are a subset is the LHCbIDs of this track.
      bool containsLhcbIDs( Track const& track ) const;

      /// Returns true if LHCbIDs in ids are a subset of the LHCbIDs of this track.
      bool containsLhcbIDs( span<LHCbID const> ids ) const;

      /// Returns the number of common LHCbIDs.
      size_t nCommonLhcbIDs( Track const& track ) const;

      /// Remove an LHCbID from the list of LHCbIDs associated to the track
      void removeFromLhcbIDs( LHCbID const& value );

      /// Set pointer to object holding track fit data. Track becomes owner.
      void setFitResult( std::unique_ptr<TrackFitResult> trackfit );

      /// get pointer to the object holding the trackfit data.
      TrackFitResult* fitResult() { return m_fitResult.ptr(); };

      /// get const pointer to the object holding the trackfit data.
      TrackFitResult const* fitResult() const { return m_fitResult.ptr(); };

      /// Check the type of the track (see the Types enum)
      bool checkType( Types const value ) const { return type() == value; };

      /// Check the history of the track (see the History enum)
      bool checkHistory( History const value ) const { return history() == value; };

      /// Check the fit history of the track (see the FitHistory enum)
      bool checkFitHistory( FitHistory const value ) const { return fitHistory() == value; };

      /// Check the pattern recognition status of the track (see the PatRecStatus enum)
      bool checkPatRecStatus( PatRecStatus const value ) const { return patRecStatus() == value; };

      /// Check the fitting status of the track (see the FitStatus enum)
      bool checkFitStatus( FitStatus const value ) const { return fitStatus() == value; };

      /// Update the flag (see the Flags enum)
      void setFlag( Flags const flag, bool const ok );

      /// Check the status of the flag (see the Flags enum)
      bool checkFlag( Flags const flag ) const;

      /// Check if track is of a type that goes thro T stations
      bool hasT() const;

      /// Check if track is of a type that goes thro Velo
      bool hasVelo() const;

      /// Check if track is of a type that goes thro TT
      bool hasTT() const;

      /// Check if track is of a type that goes thro UT
      bool hasUT() const;

      /// Retrieve the number of Measurements removed by the track fit (the number of LHCbIDs remains unchanged)
      unsigned int nMeasurementsRemoved() const { return m_fitResult ? m_fitResult->nOutliers() : 0; };

      /// Check whether the given LHCbID is on the track
      bool isOnTrack( LHCbID const value ) const;

      /// Return the measurement on the track corresponding to the input LHCbID. Call first the "isMeasurementOnTrack"
      /// method before calling this one, as it throws an exception if the LHCbID is not present! (ONLY for tracking
      /// code, not for general use.)
      const Measurement* measurement( LHCbID const value ) const
      {
        return m_fitResult ? m_fitResult->measurement( value ) : nullptr;
      };

      /// printOut method to Gaudi message stream
      std::ostream& fillStream( std::ostream& os ) const;

      /// Check whether the track has information for the specified key
      bool hasInfo( AdditionalInfo const key ) const
      {
        return m_extraInfo.end() != m_extraInfo.find( static_cast<int>( key ) );
      };

      /// Add new information, associated with the specified key. This method cannot be used to modify information for
      /// an already existing key
      bool addInfo( AdditionalInfo const key, double const info );

      /// Extract the information associated with the specified key. If there is no such information the default value
      /// will be returned.
      double info( AdditionalInfo const key, double const def ) const;

      /// Erase the information associated with the specified key
      ExtraInfo::size_type eraseInfo( AdditionalInfo const key )
      {
        return m_extraInfo.erase( static_cast<int>( key ) );
      };

      auto chi2PerDoF() const { return m_chi2PerDoF.chi2PerDoF; };

      void setChi2PerDoF( Chi2PerDoF const chi2PerDof ) { m_chi2PerDoF = chi2PerDof; };

      auto nDoF() const { return m_chi2PerDoF.nDoF; };

      /// Retrieve const  Likelihood variable
      auto likelihood() const { return m_likelihood; };

      /// Update  Likelihood variable
      void setLikelihood( double value ) { m_likelihood = value; };

      /// Retrieve const  ghost probability variable
      auto ghostProbability() const { return m_ghostProbability; };

      /// Update  ghost probability variable
      void setGhostProbability( double value ) { m_ghostProbability = value; };

      /// Retrieve const  The variety of track flags
      auto flags() const { return m_flags; };

      /// Update  The variety of track flags
      void setFlags( unsigned int value ) { m_flags = value; };

      /// Retrieve Track type
      Types type() const { return static_cast<Types>( details::getBits<flagsMasks::typeMask>( m_flags ) ); };

      /// Update Track type
      void setType( Types const value )
      {
        details::setBits<flagsMasks::typeMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve Specifies the pattern recognition algorithm that created the track
      History history() const { return static_cast<History>( details::getBits<flagsMasks::historyMask>( m_flags ) ); };

      /// Update Specifies the pattern recognition algorithm that created the track
      void setHistory( History const value )
      {
        details::setBits<flagsMasks::historyMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve Track flags
      Flags flag() const { return static_cast<Flags>( details::getBits<flagsMasks::flagMask>( m_flags ) ); };

      /// Retrieve Specifies the fitting algorithm the fitted the track)
      FitHistory fitHistory() const
      {
        return static_cast<FitHistory>( details::getBits<flagsMasks::fitHistoryMask>( m_flags ) );
      };

      /// Update Specifies the fitting algorithm the fitted the track)
      void setFitHistory( FitHistory const value )
      {
        details::setBits<flagsMasks::fitHistoryMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve Pattern recognition status of the track
      PatRecStatus patRecStatus() const
      {
        return static_cast<PatRecStatus>( details::getBits<flagsMasks::patRecStatusMask>( m_flags ) );
      };

      /// Update Pattern recognition status of the track
      void setPatRecStatus( PatRecStatus const value )
      {
        details::setBits<flagsMasks::patRecStatusMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve Fitting status of the track
      FitStatus fitStatus() const
      {
        return static_cast<FitStatus>( details::getBits<flagsMasks::fitStatusMask>( m_flags ) );
      };

      /// Update Fitting status of the track
      void setFitStatus( FitStatus const value )
      {
        details::setBits<flagsMasks::fitStatusMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve Track specific bits
      unsigned int specific() const
      {
        return static_cast<unsigned int>( details::getBits<flagsMasks::specificMask>( m_flags ) );
      };

      /// Update Track specific bits
      void setSpecific( unsigned int value )
      {
        details::setBits<flagsMasks::specificMask>( m_flags, static_cast<uint32_t>( value ) );
      };

      /// Retrieve const  Container of (sorted) LHCbIDs
      auto const& lhcbIDs() const { return m_lhcbIDs; };

      /// Retrieve const  Container with pointers to all the states
      auto const& states() const { return m_states; };

      /// Retrieve  Container of all the states
      auto& states() { return m_states; };

      /// Retrieve const  Additional pattern recognition information. Do not access directly, use *Info() methods
      /// instead.
      ExtraInfo const& extraInfo() const { return m_extraInfo; };

      /// Update  Additional pattern recognition information. Do not access directly, use *Info() methods instead.
      void setExtraInfo( ExtraInfo const& value ) { m_extraInfo = value; };

      /// Retrieve (const)  Ancestor tracks that created this one
      auto const& ancestors() const { return m_ancestors; };

      /// Retrieve  Ancestor tracks that created this one
      auto& ancestors() { return m_ancestors; };

      /// Add a track to the list of ancestors of this track
      void addToAncestors( Track const& ancestor ) { m_ancestors.push_back( &ancestor ); };

      /// Remove from  Ancestor tracks that created this one
      void removeFromAncestors( Track const* value );

      /// Clear  Ancestor tracks that created this one
      void clearAncestors() { m_ancestors.clear(); };

    protected:
    private:
      Chi2PerDoF                                  m_chi2PerDoF{};
      double                                      m_likelihood{999};       ///< Likelihood variable
      double                                      m_ghostProbability{999}; ///< ghost probability variable
      unsigned int                                m_flags{0};              ///< The variety of track flags
      std::vector<LHCbID>                         m_lhcbIDs{};             ///< Container of (sorted) LHCbIDs
      std::vector<State>                          m_states{}; ///< Container with pointers to all the states
      LHCb::cxx::PolymorphicValue<TrackFitResult> m_fitResult{
          nullptr};            ///< Transient data related to track fit (nodes, material, etc)
      ExtraInfo m_extraInfo{}; ///< Additional pattern recognition information. Do not access directly, use *Info()
                               ///methods instead.
      std::vector<Track const*> m_ancestors{}; ///< Ancestor tracks that created this one

      /// Make sure that the offset is the sum of the previous entries
      enum flagsMasks : uint32_t {
        typeMask         = details::mask_v<4, 0>,
        historyMask      = details::mask_v<7, 4>,
        fitHistoryMask   = details::mask_v<3, 11>,
        patRecStatusMask = details::mask_v<2, 14>,
        fitStatusMask    = details::mask_v<2, 16>,
        flagMask         = details::mask_v<10, 18>,
        specificMask     = details::mask_v<4, 28>
      };
      static_assert( flagsMasks::typeMask == 0xfL );
      static_assert( flagsMasks::historyMask == 0x7f0L );
      static_assert( flagsMasks::fitHistoryMask == 0x3800L );
      static_assert( flagsMasks::patRecStatusMask == 0xc000L );
      static_assert( flagsMasks::fitStatusMask == 0x30000L );
      static_assert( flagsMasks::flagMask == 0xffc0000L );
      static_assert( flagsMasks::specificMask == 0xf0000000L );

    }; // class Track

    inline std::ostream& operator<<( std::ostream& str, Track const& obj ) { return obj.fillStream( str ); }

    inline std::ostream& operator<<( std::ostream& s, Track::History e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::FitHistory e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::Types e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::PatRecStatus e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::FitStatus e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::Flags e ) { return s << toString( e ); }

    inline std::ostream& operator<<( std::ostream& s, Track::AdditionalInfo e ) { return s << toString( e ); }

    // -----------------------------------------------------------------------------
    // end of class
    // -----------------------------------------------------------------------------
  } // namespace v2
} // namespace LHCb::Event

#endif /// TrackEvent_v2_H
