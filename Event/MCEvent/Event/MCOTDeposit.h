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
#include "Event/MCHit.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/OTChannelID.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCOTDeposit = 8003;

  // Namespace for locations in TDS
  namespace MCOTDepositLocation {
    inline const std::string Default = "/Event/MC/OT/Deposits";
  }

  /** @class MCOTDeposit MCOTDeposit.h
   *
   * MCHits are converted to MCOTDeposits, which are then used to create
   * MCOTTimes
   *
   * @author Jeroen van Tilburg
   *
   */

  class MCOTDeposit : public ContainedObject {
  public:
    /// typedef for std::vector of MCOTDeposit
    typedef std::vector<MCOTDeposit*>       Vector;
    typedef std::vector<const MCOTDeposit*> ConstVector;

    /// typedef for ObjectVector of MCOTDeposit
    typedef ObjectVector<MCOTDeposit> Container;

    /// Deposit type enumerations
    enum Types {
      TypeUnknown = 0, // Unknown deposit type
      Signal,          // Signal deposit from MCHit
      Noise,           // Noise deposit
      XTalk,           // X-talk deposit
      DoublePulse      // Two hits within the same window
    };

    /// constructor with arguments
    MCOTDeposit( const LHCb::MCOTDeposit::Types& aType, const LHCb::MCHit* aMCHit, const LHCb::OTChannelID& aChannel,
                 const double& aTime, const double& aDistance, const int& ambiguity )
        : m_type( aType )
        , m_channel( aChannel )
        , m_time( aTime )
        , m_driftDistance( aDistance )
        , m_ambiguity( ambiguity )
        , m_mcHit( aMCHit ) {}

    /// Default Constructor
    MCOTDeposit()
        : m_type( LHCb::MCOTDeposit::Types::TypeUnknown )
        , m_channel()
        , m_time( 0.0 )
        , m_driftDistance( 0.0 )
        , m_ambiguity( 0 ) {}

    /// Default Destructor
    virtual ~MCOTDeposit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// conversion of string to enum for type Types
    static LHCb::MCOTDeposit::Types TypesToType( const std::string& aName );

    /// conversion to string for enum type Types
    static const std::string& TypesToString( int aEnum );

    /// add a time to the time
    void addTime( double aTime );

    /// set the TDC time in the OTChannelID
    void setTdcTime( unsigned tdcTime );

    /// get the TDC-time from the OTChannelID
    unsigned tdcTime() const;

    /// return true if signal
    bool isSignal() const;

    /// return true if noise
    bool isNoise() const;

    /// return true if XTalk
    bool isXTalk() const;

    /// return true if double pulse
    bool isDoublePulse() const;

    /// Get Non-const reference OTChannelID
    OTChannelID& channel();

    /// Retrieve const  Deposit type
    const LHCb::MCOTDeposit::Types& type() const;

    /// Update  Deposit type
    void setType( const LHCb::MCOTDeposit::Types& value );

    /// Retrieve const  OTChannelID
    const LHCb::OTChannelID& channel() const;

    /// Retrieve const  time
    double time() const;

    /// Update  time
    void setTime( double value );

    /// Retrieve const  Drift distance to wire (used for cheating)
    double driftDistance() const;

    /// Update  Drift distance to wire (used for cheating)
    void setDriftDistance( double value );

    /// Retrieve const  MC solution for the correct left/right ambiguity
    int ambiguity() const;

    /// Update  MC solution for the correct left/right ambiguity
    void setAmbiguity( int value );

    /// Retrieve (const)  Reference to MCHit that was converted to a MCOTDeposit
    const LHCb::MCHit* mcHit() const;

    friend std::ostream& operator<<( std::ostream& str, const MCOTDeposit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::MCOTDeposit::Types m_type;          ///< Deposit type
    LHCb::OTChannelID        m_channel;       ///< OTChannelID
    double                   m_time;          ///< time
    double                   m_driftDistance; ///< Drift distance to wire (used for cheating)
    int                      m_ambiguity;     ///< MC solution for the correct left/right ambiguity
    SmartRef<LHCb::MCHit>    m_mcHit;         ///< Reference to MCHit that was converted to a MCOTDeposit

    static const GaudiUtils::VectorMap<std::string, Types>& s_TypesTypMap();

  }; // class MCOTDeposit

  /// Definition of vector container type for MCOTDeposit
  typedef ObjectVector<MCOTDeposit> MCOTDeposits;

  inline std::ostream& operator<<( std::ostream& s, LHCb::MCOTDeposit::Types e ) {
    switch ( e ) {
    case LHCb::MCOTDeposit::TypeUnknown:
      return s << "TypeUnknown";
    case LHCb::MCOTDeposit::Signal:
      return s << "Signal";
    case LHCb::MCOTDeposit::Noise:
      return s << "Noise";
    case LHCb::MCOTDeposit::XTalk:
      return s << "XTalk";
    case LHCb::MCOTDeposit::DoublePulse:
      return s << "DoublePulse";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::MCOTDeposit::Types";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCOTDeposit::clID() const { return LHCb::MCOTDeposit::classID(); }

inline const CLID& LHCb::MCOTDeposit::classID() { return CLID_MCOTDeposit; }

inline std::ostream& LHCb::MCOTDeposit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "type :	" << m_type << std::endl
    << "channel :	" << m_channel << std::endl
    << "time :	" << (float)m_time << std::endl
    << "driftDistance :	" << (float)m_driftDistance << std::endl
    << "ambiguity :	" << m_ambiguity << std::endl
    << " }";
  return s;
}

inline const GaudiUtils::VectorMap<std::string, LHCb::MCOTDeposit::Types>& LHCb::MCOTDeposit::s_TypesTypMap() {
  static const GaudiUtils::VectorMap<std::string, Types> m = {{"TypeUnknown", TypeUnknown},
                                                              {"Signal", Signal},
                                                              {"Noise", Noise},
                                                              {"XTalk", XTalk},
                                                              {"DoublePulse", DoublePulse}};
  return m;
}

inline LHCb::MCOTDeposit::Types LHCb::MCOTDeposit::TypesToType( const std::string& aName ) {
  auto iter = s_TypesTypMap().find( aName );
  return iter != s_TypesTypMap().end() ? iter->second : TypeUnknown;
}

inline const std::string& LHCb::MCOTDeposit::TypesToString( int aEnum ) {
  static const std::string s_TypeUnknown = "TypeUnknown";
  auto                     iter          = std::find_if( s_TypesTypMap().begin(), s_TypesTypMap().end(),
                            [&]( const std::pair<const std::string, Types>& i ) { return i.second == aEnum; } );
  return iter != s_TypesTypMap().end() ? iter->first : s_TypeUnknown;
}

inline const LHCb::MCOTDeposit::Types& LHCb::MCOTDeposit::type() const { return m_type; }

inline void LHCb::MCOTDeposit::setType( const LHCb::MCOTDeposit::Types& value ) { m_type = value; }

inline const LHCb::OTChannelID& LHCb::MCOTDeposit::channel() const { return m_channel; }

inline double LHCb::MCOTDeposit::time() const { return m_time; }

inline void LHCb::MCOTDeposit::setTime( double value ) { m_time = value; }

inline double LHCb::MCOTDeposit::driftDistance() const { return m_driftDistance; }

inline void LHCb::MCOTDeposit::setDriftDistance( double value ) { m_driftDistance = value; }

inline int LHCb::MCOTDeposit::ambiguity() const { return m_ambiguity; }

inline void LHCb::MCOTDeposit::setAmbiguity( int value ) { m_ambiguity = value; }

inline const LHCb::MCHit* LHCb::MCOTDeposit::mcHit() const { return m_mcHit; }

inline void LHCb::MCOTDeposit::addTime( double aTime ) { m_time += aTime; }

inline void LHCb::MCOTDeposit::setTdcTime( unsigned tdcTime ) { m_channel.setTdcTime( tdcTime ); }

inline unsigned LHCb::MCOTDeposit::tdcTime() const { return channel().tdcTime(); }

inline bool LHCb::MCOTDeposit::isSignal() const { return ( Types::Signal == type() ); }

inline bool LHCb::MCOTDeposit::isNoise() const { return ( Types::Noise == type() ); }

inline bool LHCb::MCOTDeposit::isXTalk() const { return ( Types::XTalk == type() ); }

inline bool LHCb::MCOTDeposit::isDoublePulse() const { return ( Types::DoublePulse == type() ); }

inline LHCb::OTChannelID& LHCb::MCOTDeposit::channel() { return m_channel; }
