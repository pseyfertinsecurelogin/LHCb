/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Kernel/IRateFromTCK.h"
#include "OfflineDeterministicPrescaler.h"
#include "RateFromCondDB.h"
#include <iomanip>
//-----------------------------------------------------------------------------
// Implementation file for class : OfflineRateLimiter
//
// 2011-08-05 : Patrick Koppenburg
//-----------------------------------------------------------------------------
/** @class OfflineRateLimiter OfflineRateLimiter.h
 *
 *  Algorithm to ``rate limit'' a line in the stripping.
 *  It needs to calibrate to something else, i.e. an Hlt rate limited line.
 *
 * Usage:
 *
 * @verbatim
 * OfflineRateLimiter().HltLimiter = "Hlt1L0AnyRateLimitedPostScaler"
 * OfflineRateLimiter().Rate = 1. # Hz
 * @endverbatim
 *
 * Will reduce the rate of Hlt1L0AnyRateLimited to 1 Hz
 *
 * A special case is the NoBias in 2012 data:
 *
 * @verbatim
 * OfflineRateLimiter().UseCondition = True
 * OfflineRateLimiter().HltLimiter = "Hlt1MBNoBiasODINFilter"
 * Offlineratelimiter().Prescaler = "Hlt1MBNoBiasPreScaler"
 * @endverbatim
 *
 * This will first try the conditions database and if not found will go to the Hlt1MBNoBiasODINFilter
 *
 *  @author Patrick Koppenburg
 *  @date   2011-08-05
 */
class OfflineRateLimiter final : public OfflineDeterministicPrescaler {

public:
  /// Standard constructor
  using OfflineDeterministicPrescaler::OfflineDeterministicPrescaler;

  bool operator()( const LHCb::RecHeader& header ) const override;

private:
  bool       handleTCK();              ///< Check if TCK has changed
  void       updateRateFromTCK();      ///< Check if TCK has changed, update rate
  void       updatePrescaleFromTCK();  ///< Check if TCK has changed, update prescale
  void       updateRateFromCondDB();   ///< Check if CondDB has changed
  void       updateRate();             ///< Redo all calculations
  StatusCode initializeOnFirstEvent(); ///< wait for event 1 to initialize

private:
  IRateFromTCK*                m_tckReader = nullptr; ///< Property Config Service
  Gaudi::Property<std::string> m_hltLimiter{this, "HltLimiter", "Hlt1MBNoBiasODINFilter",
                                            "Reference Hlt rate limiter"}; ///< Line wrt to which one wants to scale
  Gaudi::Property<double>      m_rate{this, "Rate", 0., "Rate to achieve [Hz]"}; ///< Target rate
  unsigned int                 m_tck{0};                                         ///< Last TCK seen
  Gaudi::Property<bool>        m_useCondDB{this, "UseCondition", false};         ///< Use Condition DB
  RateFromCondDB*              m_condTriggerTool = nullptr;                      ///< Rate from CondDB
  Gaudi::Property<bool>        m_fallback{this, "FallBack",
                                   true}; ///< Fallback to TCK if rate is zero (for instance in 2011 data)
  Gaudi::Property<std::string> m_prescaler{this, "Prescaler", "Hlt1MBNoBiasPreScaler",
                                           "Name of Prescaler"}; ///< Name of prescaler, if any
  double                       m_storedRate{0};                  ///< Rate stored in TCK or CondDB
  double                       m_storedPrescale{1};              ///< Prescale stored in TCK
  bool                         m_initialized{false};             ///< has been initialized

  mutable std::mutex m_lock;
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OfflineRateLimiter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode OfflineRateLimiter::initializeOnFirstEvent() {
  StatusCode sc = StatusCode::SUCCESS;
  if ( m_initialized ) return sc;
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;
  m_initialized = true;
  if ( m_rate < 0 ) Exception( "Negative Rate requested" );
  if ( m_useCondDB.value() ) {
    m_condTriggerTool = tool<RateFromCondDB>( "RateFromCondDB", this );
    if ( !m_condTriggerTool ) {
      warning() << "Failed to initialise RateFromCondDB. This is expected on MC." << endmsg;
      m_useCondDB = false;
    } else {
      if ( m_condTriggerTool->initializeCondDB() ) {
        if ( !m_prescaler.empty() ) {
          m_tckReader = tool<IRateFromTCK>( "RateFromTCK", this ); // make it private
          sc          = m_tckReader->runUpdate();
          if ( !sc ) {
            warning() << "Failed to update condDB for RateFromTCK. Will assume there is no preccale." << endmsg;
            release( m_tckReader ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
            m_tckReader = nullptr;
          }
        }
      } else if ( m_fallback.value() ) {
        info() << "Failed to get rate from condition database. This is normal for <2012." << endmsg;
        info() << "Fill fallback on TCK" << endmsg;
        release( m_condTriggerTool ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        m_useCondDB = false;
      } else {
        Exception( "Failed to get rate from condition database and no fallback" );
      }
    }
  }
  // this one I need anyway as fallback
  if ( !m_useCondDB.value() || m_fallback.value() ) {
    m_tckReader = tool<IRateFromTCK>( "RateFromTCK", this ); // make it private
    if ( !m_tckReader ) return StatusCode::FAILURE;
    sc = m_tckReader->runUpdate();
    if ( !sc ) {
      warning() << "Failed to update condDB for RateFromTCK. This is OK on MC. Setting rate ot 0." << endmsg;
      m_useCondDB = false;
      m_rate      = 0;
    }
  }
  updateRate();

  return StatusCode::SUCCESS;
}
//=============================================================================
// Not an Incident
//=============================================================================
bool OfflineRateLimiter::handleTCK() {
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> handleTCK " << endmsg;
  if ( !m_tckReader ) Exception( "TCK Reader is NULL in handleTCK" );
  const unsigned int newTCK = m_tckReader->getTCK();
  if ( m_tck == newTCK ) return false;

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
    debug() << " changed TCK from " << std::hex << m_tck << " to " << newTCK << endmsg;
  m_tck = newTCK;
  return true;
}
//=============================================================================
//
//=============================================================================
void OfflineRateLimiter::updateRate() {
  const double triggerRate = ( m_storedRate * m_storedPrescale );
  const double newacc      = ( triggerRate > 0 ) ? m_rate / triggerRate : 0.;
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> updateRate " << newacc << endmsg;
  if ( m_accFrac != newacc ) {
    info() << "Rate is now " << m_storedRate << " Hz and precale " << m_storedPrescale << " (=" << triggerRate << " Hz)"
           << " -> Need to reduce by factor " << newacc << " to reach target " << m_rate.value() << " Hz." << endmsg;
    m_accFrac = newacc;
    update(); // update in baseclass
  }
}
//=============================================================================
//
//=============================================================================
void OfflineRateLimiter::updateRateFromTCK() {
  if ( handleTCK() ) {
    const double randomRate = m_tckReader->rateFromTCK( m_hltLimiter );
    if ( randomRate <= 0 ) {
      warning() << "Rate of " << m_hltLimiter.value() << " in TCK " << std::hex << m_tck << " is " << randomRate
                << " Hz" << endmsg;
    }
    m_storedRate = randomRate;
    updateRate();
  }
}
//=============================================================================
//
//=============================================================================
void OfflineRateLimiter::updatePrescaleFromTCK() {
  if ( handleTCK() ) {
    const double prescale = m_tckReader->prescaleFromTCK( m_prescaler.value() );
    if ( prescale <= 0 ) {
      warning() << "Prescale of " << m_prescaler.value() << " in TCK " << std::hex << m_tck << " is " << prescale
                << endmsg;
    }
    m_storedPrescale = prescale;
    updateRate();
  }
}
//=============================================================================
// Not an Incident
//=============================================================================
void OfflineRateLimiter::updateRateFromCondDB() {
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> updateRateFromCondDB " << endmsg;
  if ( !m_condTriggerTool ) Exception( "CondDB Reader is NULL in updateRateFromCondDB" );

  const double randomRate = m_condTriggerTool->getRate();
  if ( randomRate > 0 ) {
    m_storedRate = randomRate;
    if ( m_tckReader ) {
      updatePrescaleFromTCK();
    } else {
      updateRate();
    }
  }
}

//=============================================================================
// Main execution
//=============================================================================
bool OfflineRateLimiter::operator()( const LHCb::RecHeader& header ) const {
  auto lock = std::scoped_lock{m_lock};
  auto self = const_cast<OfflineRateLimiter*>( this );
  //  somewhere here get rate
  if ( !self->initializeOnFirstEvent() ) throw StatusCode::FAILURE;
  if ( m_useCondDB.value() ) {
    self->updateRateFromCondDB();
  } else {
    self->updateRateFromTCK();
  }
  return static_cast<OfflineDeterministicPrescaler const&>( *this )( header );
}
